#include "persistent_http.h"

#include <stdexcept>
#include <cstring>

namespace
{
    std::size_t writeToString(
        char* pointer,
        std::size_t size,
        std::size_t count,
        void* userData
    )
    {
        auto* text = static_cast<std::string*>(userData);
        text->append(pointer, size * count);
        return size * count;
    }
}

PersistentHttpClient::PersistentHttpClient()
    : handle(nullptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    handle = curl_easy_init();
    if (handle == nullptr)
    {
        throw std::runtime_error("curl_easy_init failed");
    }
    _timeout = 70;
}

PersistentHttpClient::~PersistentHttpClient()
{
    if (handle != nullptr)
    {
        curl_easy_cleanup(handle);
    }
}

void PersistentHttpClient::applyFastOptions() const
{
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(
        handle,
        CURLOPT_IPRESOLVE,
        CURL_IPRESOLVE_V4
    );
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, _timeout);
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(
        handle,
        CURLOPT_DNS_CACHE_TIMEOUT,
        300L
    );
    curl_easy_setopt(
        handle,
        CURLOPT_HTTP_VERSION,
        CURL_HTTP_VERSION_1_1
    );
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(
        handle,
        CURLOPT_SSL_VERIFYPEER,
        0L
    );
    curl_easy_setopt(
        handle,
        CURLOPT_SSL_VERIFYHOST,
        0L
    );
}

int PersistentHttpClient::getRequestMaxRetries() const
{
    return 1;
}

int PersistentHttpClient::getRequestBackoff() const
{
    return 0;
}

std::string PersistentHttpClient::makeRequest(
    const TgBot::Url& url,
    const std::vector<TgBot::HttpReqArg>& args
) const
{
    curl_easy_reset(handle);
    applyFastOptions();

    std::string address =
        url.protocol + "://" + url.host + url.path;
    if (args.empty())
    {
        address += "?" + url.query;
    }
    curl_easy_setopt(
        handle,
        CURLOPT_URL,
        address.c_str()
    );

    curl_mime* mime = curl_mime_init(handle);
    if (!args.empty())
    {
        for (const auto& argument : args)
        {
            curl_mimepart* part =
                curl_mime_addpart(mime);
            curl_mime_data(
                part,
                argument.value.c_str(),
                argument.value.size()
            );
            curl_mime_type(
                part,
                argument.mimeType.c_str()
            );
            curl_mime_name(
                part,
                argument.name.c_str()
            );
            if (argument.isFile)
            {
                curl_mime_filename(
                    part,
                    argument.fileName.c_str()
                );
            }
        }
        curl_easy_setopt(handle, CURLOPT_MIMEPOST, mime);
    }

    std::string response;
    curl_easy_setopt(
        handle,
        CURLOPT_WRITEDATA,
        &response
    );
    curl_easy_setopt(
        handle,
        CURLOPT_WRITEFUNCTION,
        writeToString
    );

    char errorBuffer[CURL_ERROR_SIZE] {};
    curl_easy_setopt(
        handle,
        CURLOPT_ERRORBUFFER,
        errorBuffer
    );

    const CURLcode result = curl_easy_perform(handle);
    curl_mime_free(mime);

    if (result != CURLE_OK)
    {
        std::string message = errorBuffer;
        if (message.empty())
        {
            message = curl_easy_strerror(result);
        }
        throw std::runtime_error(
            std::string("curl error: ") + message
        );
    }

    return parser.extractBody(response);
}
