#ifndef N_STATE_PERSISTENT_HTTP_H
#define N_STATE_PERSISTENT_HTTP_H

#include <tgbot/net/HttpClient.h>
#include <tgbot/net/HttpParser.h>

#include <curl/curl.h>

#include <string>
#include <vector>

class PersistentHttpClient : public TgBot::HttpClient
{
public:
    PersistentHttpClient();
    PersistentHttpClient(const PersistentHttpClient&) =
        delete;
    ~PersistentHttpClient() override;

    PersistentHttpClient& operator=(
        const PersistentHttpClient&
    ) = delete;

    std::string makeRequest(
        const TgBot::Url& url,
        const std::vector<TgBot::HttpReqArg>& args
    ) const override;

    int getRequestMaxRetries() const override;
    int getRequestBackoff() const override;

private:
    void applyFastOptions() const;

    mutable CURL* handle;
    TgBot::HttpParser parser;
};

#endif
