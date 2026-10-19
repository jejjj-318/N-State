#ifndef N_STATE_TG_BOT_H
#define N_STATE_TG_BOT_H

#include "../country/country.h"
#include "../game/game.h"
#include "persistent_http.h"

#include <tgbot/tgbot.h>

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

class TelegramBot
{
private:
    enum class Screen
    {
        Difficulty,
        Main,
        Statistics,
        Economy,
        Social,
        Political,
        Funding,
        Finished
    };

    struct PlayerState
    {
        Game game;

        std::int64_t controlChatId = 0;
        std::int32_t controlMessageId = 0;

        Screen currentScreen = Screen::Difficulty;
        FundingArea currentFundingArea =
            FundingArea::Industry;
        bool busy = false;
    };

    PersistentHttpClient httpClient;
    TgBot::Bot bot;

    std::unordered_map<std::int64_t, PlayerState> players;

public:
    explicit TelegramBot(const std::string& token);

    void run();

private:
    static std::string formatDate(
        const std::chrono::year_month_day& date
    );

    static std::optional<Difficulty> parseDifficulty(
        const std::string& data
    );

    static std::optional<FundingArea> parseFundingArea(
        const std::string& data
    );

    static TgBot::InlineKeyboardMarkup::Ptr
    createDifficultyKeyboard();

    static TgBot::InlineKeyboardMarkup::Ptr
    createMainKeyboard();

    static TgBot::InlineKeyboardMarkup::Ptr
    createEconomyKeyboard();

    static TgBot::InlineKeyboardMarkup::Ptr
    createSocialKeyboard();

    static TgBot::InlineKeyboardMarkup::Ptr
    createPoliticalKeyboard();

    static TgBot::InlineKeyboardMarkup::Ptr
    createFundingKeyboard(FundingArea area);

    static TgBot::InlineKeyboardMarkup::Ptr
    createBackKeyboard(const std::string& callbackData);

    static std::string buildMainMenuText(
        const Game& game
    );

    static std::string fundingName(FundingArea area);

    static std::string fundingDescription(
        FundingArea area
    );

    void handleStart(TgBot::Message::Ptr message);

    void handleCallback(
        TgBot::CallbackQuery::Ptr query
    );

    void handleDifficulty(
        PlayerState& player,
        Difficulty difficulty
    );

    void answerQuery(
        const TgBot::CallbackQuery::Ptr& query
    );

    bool editMessage(
        std::int64_t chatId,
        std::int32_t messageId,
        const std::string& text,
        TgBot::InlineKeyboardMarkup::Ptr keyboard
    );

    bool editControlMessage(
        PlayerState& player,
        const std::string& text,
        TgBot::InlineKeyboardMarkup::Ptr keyboard,
        Screen newScreen
    );

    bool sendMainMenu(PlayerState& player);

    bool showMainMenu(PlayerState& player);

    bool showStatistics(PlayerState& player);

    bool showEconomy(PlayerState& player);

    bool showSocial(PlayerState& player);

    bool showPolitical(PlayerState& player);

    bool showFunding(
        PlayerState& player,
        FundingArea area,
        const std::string& notice = ""
    );

    bool showFinished(PlayerState& player);
};

#endif
