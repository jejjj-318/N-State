#include "tg_bot.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

namespace Cb
{
    constexpr const char* DifficultyUnstable = "difficulty_unstable"; // сделать ключи
    constexpr const char* DifficultyCrisis = "difficulty_crisis";
    constexpr const char* DifficultyCollapse = "difficulty_collapse";

    constexpr const char* Statistics = "menu_statistics";
    constexpr const char* Economy = "menu_economy";
    constexpr const char* Social = "menu_social";
    constexpr const char* Political = "menu_political";
    constexpr const char* NextMonth = "next_month";

    constexpr const char* Industry = "fund_industry";
    constexpr const char* Agriculture = "fund_agriculture";
    constexpr const char* Education = "fund_education";
    constexpr const char* Healthcare = "fund_healthcare";
    constexpr const char* Defense = "fund_defense";
    constexpr const char* Administration = "fund_administration";

    constexpr const char* UpIndustry = "fund_up_industry";
    constexpr const char* DownIndustry = "fund_down_industry";
    constexpr const char* UpAgriculture = "fund_up_agriculture";
    constexpr const char* DownAgriculture = "fund_down_agriculture";
    constexpr const char* UpEducation = "fund_up_education";
    constexpr const char* DownEducation = "fund_down_education";
    constexpr const char* UpHealthcare = "fund_up_healthcare";
    constexpr const char* DownHealthcare = "fund_down_healthcare";
    constexpr const char* UpDefense = "fund_up_defense";
    constexpr const char* DownDefense = "fund_down_defense";
    constexpr const char* UpAdministration = "fund_up_administration";
    constexpr const char* DownAdministration = "fund_down_administration";

    constexpr const char* BackMain = "back_main";
    constexpr const char* BackEconomy = "back_economy";
    constexpr const char* BackSocial = "back_social";
    constexpr const char* BackPolitical = "back_political";
}

namespace
{
    TgBot::InlineKeyboardButton::Ptr makeButton(
        const std::string& text,
        const std::string& data
    )
    {
        auto button =
            std::make_shared<TgBot::InlineKeyboardButton>();

        button->text = text;
        button->callbackData = data;

        return button;
    }

    TgBot::InlineKeyboardMarkup::Ptr makeKeyboard(
        const std::vector<std::pair<std::string, std::string>>& rows
    )
    {
        auto keyboard =
            std::make_shared<TgBot::InlineKeyboardMarkup>();

        for (const auto& [text, data] : rows)
        {
            keyboard->inlineKeyboard.push_back(
                {makeButton(text, data)}
            );
        }

        return keyboard;
    }

    std::string formatNumber(double value)
    {
        std::ostringstream stream;

        stream << std::fixed
               << std::setprecision(2)
               << value;

        return stream.str();
    }

    bool startsWith(
        const std::string& value,
        const std::string& prefix
    )
    {
        return value.rfind(prefix, 0) == 0;
    }
}

TelegramBot::TelegramBot(const std::string& token)
    : httpClient(),
      bot(token, httpClient)
{
}

std::string TelegramBot::formatDate(
    const std::chrono::year_month_day& date
)
{
    const auto day = unsigned(date.day());
    const auto month = unsigned(date.month());
    const auto year = int(date.year());

    return
        (day < 10 ? "0" : "") +
        std::to_string(day) +
        "." +
        (month < 10 ? "0" : "") +
        std::to_string(month) +
        "." +
        std::to_string(year);
}

std::optional<Difficulty> TelegramBot::parseDifficulty(
    const std::string& data
)
{
    if (data == Cb::DifficultyUnstable)
        return Difficulty::Unstable;

    if (data == Cb::DifficultyCrisis)
        return Difficulty::Crisis;

    if (data == Cb::DifficultyCollapse)
        return Difficulty::Collapsing;

    return std::nullopt;
}

std::optional<FundingArea> TelegramBot::parseFundingArea(
    const std::string& data
)
{
    if (data == Cb::Industry ||
        data == Cb::UpIndustry ||
        data == Cb::DownIndustry)
        return FundingArea::Industry;

    if (data == Cb::Agriculture ||
        data == Cb::UpAgriculture ||
        data == Cb::DownAgriculture)
        return FundingArea::Agriculture;

    if (data == Cb::Education ||
        data == Cb::UpEducation ||
        data == Cb::DownEducation)
        return FundingArea::Education;

    if (data == Cb::Healthcare ||
        data == Cb::UpHealthcare ||
        data == Cb::DownHealthcare)
        return FundingArea::Healthcare;

    if (data == Cb::Defense ||
        data == Cb::UpDefense ||
        data == Cb::DownDefense)
        return FundingArea::Defense;

    if (data == Cb::Administration ||
        data == Cb::UpAdministration ||
        data == Cb::DownAdministration)
        return FundingArea::Administration;

    return std::nullopt;
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createDifficultyKeyboard()
{
    return makeKeyboard({
        {"Нестабильное государство", Cb::DifficultyUnstable},
        {"Государство в кризисе", Cb::DifficultyCrisis},
        {"Распадающееся государство", Cb::DifficultyCollapse}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createMainKeyboard()
{
    return makeKeyboard({
        {"Подробная статистика", Cb::Statistics},
        {"Экономическая сфера", Cb::Economy},
        {"Социально-культурная сфера", Cb::Social},
        {"Административно-политическая сфера", Cb::Political},
        {"Следующий месяц", Cb::NextMonth}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createEconomyKeyboard()
{
    return makeKeyboard({
        {"Промышленность", Cb::Industry},
        {"Сельское хозяйство", Cb::Agriculture},
        {"← Назад", Cb::BackMain}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createSocialKeyboard()
{
    return makeKeyboard({
        {"Образование", Cb::Education},
        {"Здравоохранение", Cb::Healthcare},
        {"← Назад", Cb::BackMain}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createPoliticalKeyboard()
{
    return makeKeyboard({
        {"Оборона", Cb::Defense},
        {"Государственное управление", Cb::Administration},
        {"← Назад", Cb::BackMain}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createFundingKeyboard(
    FundingArea area
)
{
    const char* up = "";
    const char* down = "";
    const char* back = Cb::BackMain;

    switch (area)
    {
        case FundingArea::Industry:
            up = Cb::UpIndustry;
            down = Cb::DownIndustry;
            back = Cb::BackEconomy;
            break;

        case FundingArea::Agriculture:
            up = Cb::UpAgriculture;
            down = Cb::DownAgriculture;
            back = Cb::BackEconomy;
            break;

        case FundingArea::Education:
            up = Cb::UpEducation;
            down = Cb::DownEducation;
            back = Cb::BackSocial;
            break;

        case FundingArea::Healthcare:
            up = Cb::UpHealthcare;
            down = Cb::DownHealthcare;
            back = Cb::BackSocial;
            break;

        case FundingArea::Defense:
            up = Cb::UpDefense;
            down = Cb::DownDefense;
            back = Cb::BackPolitical;
            break;

        case FundingArea::Administration:
            up = Cb::UpAdministration;
            down = Cb::DownAdministration;
            back = Cb::BackPolitical;
            break;
    }

    return makeKeyboard({
        {"Увеличить финансирование на 10%", up},
        {"Уменьшить финансирование на 10%", down},
        {"← Назад", back}
    });
}

TgBot::InlineKeyboardMarkup::Ptr
TelegramBot::createBackKeyboard(
    const std::string& callbackData
)
{
    return makeKeyboard({
        {"← Назад", callbackData}
    });
}

std::string TelegramBot::fundingName(
    FundingArea area
)
{
    switch (area)
    {
        case FundingArea::Industry:
            return "ПРОМЫШЛЕННОСТЬ";
        case FundingArea::Agriculture:
            return "СЕЛЬСКОЕ ХОЗЯЙСТВО";
        case FundingArea::Education:
            return "ОБРАЗОВАНИЕ";
        case FundingArea::Healthcare:
            return "ЗДРАВООХРАНЕНИЕ";
        case FundingArea::Defense:
            return "ОБОРОНА";
        case FundingArea::Administration:
            return "ГОСУДАРСТВЕННОЕ УПРАВЛЕНИЕ";
    }

    return "НАПРАВЛЕНИЕ";
}

std::string TelegramBot::fundingDescription(
    FundingArea area
)
{
    switch (area)
    {
        case FundingArea::Industry:
            return "Промышленное производство и занятость.";

        case FundingArea::Agriculture:
            return "Сельскохозяйственное производство и продовольствие.";

        case FundingArea::Education:
            return "Образование и развитие человеческого капитала.";

        case FundingArea::Healthcare:
            return "Здравоохранение и состояние населения.";

        case FundingArea::Defense:
            return "Оборона и обеспечение безопасности государства.";

        case FundingArea::Administration:
            return "Работа государственного аппарата.";
    }

    return "";
}

std::string TelegramBot::buildMainMenuText(
    const Game& game
)
{
    const Country& country = game.country();
    std::string text =
        "УПРАВЛЕНИЕ ГОСУДАРСТВОМ\n\n"
        "Текущая дата: " +
        formatDate(country.getCurrentDate()) +

        "\n"
        "Ход: " +
        std::to_string(country.getTurn()) +
        " / " +
        std::to_string(Country::maxTurns()) +
        "\n\n"

        "ВВП: " +
        formatNumber(country.getGdp()) +
        " трлн nollar\n"

        "Расходы бюджета: " +
        formatNumber(country.getBudgetExpenditure()) +
        " трлн nollar\n"

        "Инфляция: " +
        formatNumber(country.getInflation()) +
        " %\n"

        "Безработица: " +
        formatNumber(country.getUnemployment()) +
        " %\n"

        "Поддержка правительства: " +
        formatNumber(country.getGovernmentSupport()) +
        " %\n\n";

    if (game.lastEventText())
    {
        text += "Событие: ";
        text += *game.lastEventText();
        text += "\n\n";
    }

    text += "Выберите действие:";
    return text;
}

void TelegramBot::handleStart(
    TgBot::Message::Ptr message
)
{
    if (message == nullptr ||
        message->from == nullptr ||
        message->chat == nullptr)
    {
        return;
    }

    if (message->chat->type != TgBot::Chat::Type::Private)
        return;

    const std::int64_t userId = message->from->id;
    auto& player = players[userId];

    if (player.game.isStarted())
    {
        if (player.currentScreen == Screen::Finished)
        {
            showFinished(player);
            return;
        }

        if (player.controlMessageId != 0)
        {
            if (showMainMenu(player))
                return;

            player.controlMessageId = 0;
        }

        sendMainMenu(player);
        return;
    }

    if (player.controlMessageId != 0)
        return;

    try
    {
        auto controlMessage =
            bot.getApi().sendMessage(
                message->chat->id,
                "Добро пожаловать в Country N!\n\n"
                "Вам передали государство N.\n\n"
                "Выберите исходное состояние государства:",
                nullptr,
                nullptr,
                createDifficultyKeyboard()
            );

        player.controlChatId = message->chat->id;
        player.controlMessageId = controlMessage->messageId;
        player.currentScreen = Screen::Difficulty;
    }
    catch (const TgBot::TgException& exception)
    {
        std::cerr
            << "Telegram /start error: "
            << exception.what()
            << '\n';
    }
}

bool TelegramBot::editMessage(
    std::int64_t chatId,
    std::int32_t messageId,
    const std::string& text,
    TgBot::InlineKeyboardMarkup::Ptr keyboard
)
{
    if (chatId == 0 || messageId == 0)
        return false;

    try
    {
        bot.getApi().editMessageText(text, chatId, messageId,
            "",
            "",
            nullptr,
            keyboard);

        return true;
    }
    catch (const TgBot::TgException& exception)
    {
        const std::string error = exception.what();

        if (error.find("message is not modified") !=
            std::string::npos)
        {
            return true;
        }

        std::cerr
            << "Telegram edit error: "
            << error
            << '\n';

        return false;
    }
}

bool TelegramBot::editControlMessage(
    PlayerState& player,
    const std::string& text,
    TgBot::InlineKeyboardMarkup::Ptr keyboard,
    Screen newScreen
)
{
    if (!editMessage(
            player.controlChatId,
            player.controlMessageId,
            text,
            keyboard))
    {
        return false;
    }

    player.currentScreen = newScreen;
    return true;
}

void TelegramBot::answerQuery(
    const TgBot::CallbackQuery::Ptr& query
)
{
    if (query == nullptr)
    {
        return;
    }

    try
    {
        bot.getApi().answerCallbackQuery(query->id);
    }
    catch (const std::exception&)
    {
    }
}

bool TelegramBot::sendMainMenu(
    PlayerState& player
)
{
    try
    {
        auto controlMessage =
            bot.getApi().sendMessage(
                player.controlChatId,
                buildMainMenuText(player.game),
                nullptr,
                nullptr,
                createMainKeyboard()
            );

        player.controlMessageId = controlMessage->messageId;
        player.currentScreen = Screen::Main;

        return true;
    }
    catch (const TgBot::TgException& exception)
    {
        std::cerr
            << "Telegram main menu error: "
            << exception.what()
            << '\n';

        return false;
    }
}

bool TelegramBot::showMainMenu(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    return editControlMessage(
        player,
        buildMainMenuText(player.game),
        createMainKeyboard(),
        Screen::Main
    );
}

bool TelegramBot::showStatistics(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    const std::string text =
        "ПОДРОБНАЯ СТАТИСТИКА\n\n"
        "Раздел находится в разработке.\n\n"
        "Основные показатели доступны "
        "на экране управления.";

    return editControlMessage(
        player,
        text,
        createBackKeyboard(Cb::BackMain),
        Screen::Statistics
    );
}

bool TelegramBot::showEconomy(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    const auto& country = player.game.country();

    const std::string text =
        "ЭКОНОМИЧЕСКАЯ СФЕРА\n\n"
        "Выберите направление:\n\n"

        "Промышленность: " +
        formatNumber(
            country.getFunding(FundingArea::Industry)
        ) +
        " трлн nollar\n"

        "Сельское хозяйство: " +
        formatNumber(
            country.getFunding(FundingArea::Agriculture)
        ) +
        " трлн nollar";

    return editControlMessage(
        player,
        text,
        createEconomyKeyboard(),
        Screen::Economy
    );
}

bool TelegramBot::showSocial(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    const auto& country = player.game.country();

    const std::string text =
        "СОЦИАЛЬНО-КУЛЬТУРНАЯ СФЕРА\n\n"
        "Выберите направление:\n\n"

        "Образование: " +
        formatNumber(
            country.getFunding(FundingArea::Education)
        ) +
        " трлн nollar\n"

        "Здравоохранение: " +
        formatNumber(
            country.getFunding(FundingArea::Healthcare)
        ) +
        " трлн nollar";

    return editControlMessage(
        player,
        text,
        createSocialKeyboard(),
        Screen::Social
    );
}

bool TelegramBot::showPolitical(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    const auto& country = player.game.country();

    const std::string text =
        "АДМИНИСТРАТИВНО-ПОЛИТИЧЕСКАЯ СФЕРА\n\n"
        "Выберите направление:\n\n"

        "Оборона: " +
        formatNumber(
            country.getFunding(FundingArea::Defense)
        ) +
        " трлн nollar\n"

        "Государственное управление: " +
        formatNumber(
            country.getFunding(FundingArea::Administration)
        ) +
        " трлн nollar";

    return editControlMessage(
        player,
        text,
        createPoliticalKeyboard(),
        Screen::Political
    );
}

bool TelegramBot::showFunding(
    PlayerState& player,
    FundingArea area,
    const std::string& notice
)
{
    if (!player.game.isStarted())
        return false;

    player.currentFundingArea = area;

    const auto& country = player.game.country();

    std::string text =
        fundingName(area) +
        "\n\n" +
        fundingDescription(area) +
        "\n\n"
        "Текущее финансирование: " +
        formatNumber(country.getFunding(area)) +
        " трлн nollar\n\n";

    if (!notice.empty())
    {
        text += notice;
        text += "\n\n";
    }

    text += "Выберите решение:";

    return editControlMessage(
        player,
        text,
        createFundingKeyboard(area),
        Screen::Funding
    );
}

bool TelegramBot::showFinished(
    PlayerState& player
)
{
    if (!player.game.isStarted())
        return false;

    const auto& country = player.game.country();

    const std::string text =
        "ИГРА ЗАВЕРШЕНА\n\n"
        "Вы завершили 72 месяца управления государством N.\n\n"

        "Финальная дата: " +
        formatDate(country.getCurrentDate()) +

        "\n\n"
        "Финальные показатели:\n"

        "ВВП: " +
        formatNumber(country.getGdp()) +
        " трлн nollar\n"

        "Расходы бюджета: " +
        formatNumber(country.getBudgetExpenditure()) +
        " трлн nollar\n"

        "Инфляция: " +
        formatNumber(country.getInflation()) +
        " %\n"

        "Безработица: " +
        formatNumber(country.getUnemployment()) +
        " %\n"

        "Поддержка правительства: " +
        formatNumber(country.getGovernmentSupport()) +
        " %";

    player.currentScreen = Screen::Finished;

    return editMessage(
        player.controlChatId,
        player.controlMessageId,
        text,
        nullptr
    );
}

void TelegramBot::handleDifficulty(
    PlayerState& player,
    Difficulty difficulty
)
{
    if (player.currentScreen != Screen::Difficulty)
        return;

    if (player.game.isStarted())
        return;

    const std::int64_t oldChatId = player.controlChatId;
    const std::int32_t oldMessageId = player.controlMessageId;

    player.game.start("N", difficulty);

    const std::string dateText =
        formatDate(player.game.country().getStartDate());

    if (!sendMainMenu(player))
        return;

    const std::string informationText =
        "Государство N создано!\n\n"
        "Дата начала управления: " +
        dateText +
        "\n\n"
        "Исходное состояние государства выбрано.";

    const bool informationEdited =
        editMessage(
            oldChatId,
            oldMessageId,
            informationText,
            nullptr
        );

    if (!informationEdited)
    {
        try
        {
            bot.getApi().sendMessage(oldChatId, informationText);
        }
        catch (const TgBot::TgException& exception)
        {
            std::cerr
                << "Telegram information message error: "
                << exception.what()
                << '\n';
        }
    }
}

void TelegramBot::handleCallback(
    TgBot::CallbackQuery::Ptr query
)
{
    try
    {
        if (query == nullptr ||
            query->from == nullptr ||
            query->message == nullptr ||
            query->message->chat == nullptr)
        {
            return;
        }

        std::cerr
            << "\n[CALLBACK] received: "
            << query->data
            << '\n';

        const std::int64_t userId = query->from->id;

        auto playerIt = players.find(userId);

        if (playerIt == players.end())
            return;

        PlayerState& player = playerIt->second;

        if (query->message->chat->type !=
            TgBot::Chat::Type::Private)
        {
            return;
        }

        if (query->message->chat->id !=
            player.controlChatId)
        {
            return;
        }

        if (query->message->messageId !=
            player.controlMessageId)
        {
            std::cerr
                << "[CALLBACK] old message ignored\n";
            return;
        }

        if (const auto difficulty =
                parseDifficulty(query->data))
        {
            handleDifficulty(player, *difficulty);
            return;
        }

        if (query->data == Cb::Statistics)
        {
            if (player.currentScreen == Screen::Main)
                showStatistics(player);

            return;
        }

        if (query->data == Cb::Economy)
        {
            if (player.currentScreen == Screen::Main)
                showEconomy(player);

            return;
        }

        if (query->data == Cb::Social)
        {
            if (player.currentScreen == Screen::Main)
                showSocial(player);

            return;
        }

        if (query->data == Cb::Political)
        {
            if (player.currentScreen == Screen::Main)
                showPolitical(player);

            return;
        }

        if (const auto area =
                parseFundingArea(query->data))
        {
            const bool isUp =
                startsWith(query->data, "fund_up_");

            const bool isDown =
                startsWith(query->data, "fund_down_");

            const bool isSelection =
                query->data == Cb::Industry ||
                query->data == Cb::Agriculture ||
                query->data == Cb::Education ||
                query->data == Cb::Healthcare ||
                query->data == Cb::Defense ||
                query->data == Cb::Administration;

            if (isSelection)
            {
                if (*area == FundingArea::Industry ||
                    *area == FundingArea::Agriculture)
                {
                    if (player.currentScreen == Screen::Economy)
                        showFunding(player, *area);
                }
                else if (*area == FundingArea::Education ||
                         *area == FundingArea::Healthcare)
                {
                    if (player.currentScreen == Screen::Social)
                        showFunding(player, *area);
                }
                else
                {
                    if (player.currentScreen == Screen::Political)
                        showFunding(player, *area);
                }

                return;
            }

            if (player.currentScreen != Screen::Funding)
                return;

            if (!isUp && !isDown)
                return;

            const double multiplier =
                isUp ? 1.10 : 0.90;

            if (player.game.country().changeFunding(
                    *area,
                    multiplier))
            {
                const std::string notice =
                    isUp
                        ? "Финансирование увеличено на 10%."
                        : "Финансирование уменьшено на 10%.";

                showFunding(
                    player,
                    *area,
                    notice +
                    "\nИзменение показателей произойдёт "
                    "при переходе на следующий месяц."
                );
            }

            return;
        }

        if (query->data == Cb::NextMonth)
        {
            if (player.currentScreen != Screen::Main || !player.game.isStarted())
            {
                return;
            }

            if (player.game.isFinished())
            {
                showFinished(player);
                return;
            }

            player.game.nextMonth();

            if (player.game.isFinished())
            {
                showFinished(player);
            }
            else
            {
                showMainMenu(player);
            }

            return;
        }

        if (query->data == Cb::BackMain)
        {
            if (!player.game.isStarted())
                return;

            if (player.currentScreen == Screen::Economy ||
                player.currentScreen == Screen::Social ||
                player.currentScreen == Screen::Political ||
                player.currentScreen == Screen::Statistics)
            {
                showMainMenu(player);
            }

            return;
        }

        if (query->data == Cb::BackEconomy)
        {
            if (player.currentScreen == Screen::Funding)
                showEconomy(player);

            return;
        }

        if (query->data == Cb::BackSocial)
        {
            if (player.currentScreen == Screen::Funding)
                showSocial(player);

            return;
        }

        if (query->data == Cb::BackPolitical)
        {
            if (player.currentScreen == Screen::Funding)
                showPolitical(player);

            return;
        }

        std::cerr
            << "[CALLBACK] unknown callback: "
            << query->data
            << '\n';
    }
    catch (const TgBot::TgException& exception)
    {
        std::cerr
            << "[CALLBACK] Telegram error: "
            << exception.what()
            << '\n';
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "[CALLBACK] Standard error: "
            << exception.what()
            << '\n';
    }
}

void TelegramBot::run()
{
    bot.getEvents().onCommand(
        "start",
        [this](TgBot::Message::Ptr message)
        {
            handleStart(message);
        }
    );

    bot.getEvents().onCallbackQuery(
        [this](TgBot::CallbackQuery::Ptr query)
        {
            handleCallback(query);
        }
    );

    std::cout << "Telegram bot started!\n";

    TgBot::TgLongPoll longPoll(bot);

    while (true)
    {
        try
        {
            longPoll.start();
        }
        catch (const TgBot::TgException& exception)
        {
            std::cerr
                << "Telegram long poll error: "
                << exception.what()
                << '\n';

            std::this_thread::sleep_for(
                std::chrono::seconds(2)
            );
        }
        catch (const std::exception& exception)
        {
            std::cerr
                << "Standard long poll error: "
                << exception.what()
                << '\n';

            std::this_thread::sleep_for(
                std::chrono::seconds(2)
            );
        }
    }
}
