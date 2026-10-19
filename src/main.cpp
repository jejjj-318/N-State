#include "telegram/tg_bot.h"

#include <cstdlib>
#include <iostream>

int main()
{
    const char* token = std::getenv("BOT_TOKEN");

    if (token == nullptr)
    {
        std::cerr << "BOT_TOKEN is not set!\n";
        return 1;
    }

    TelegramBot bot(token);
    bot.run();

    return 0;
}