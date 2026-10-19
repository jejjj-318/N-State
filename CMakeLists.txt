cmake_minimum_required(VERSION 3.20)

project(N_State)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TgBot CONFIG REQUIRED)
find_package(Boost REQUIRED)

# Основное
add_executable(N_State
        src/main.cpp

        src/core/entity.cpp
        src/core/entity.h

        src/country/country.cpp
        src/country/country.h
        src/country/funding.cpp
        src/country/funding.h

        src/game/game.cpp
        src/game/game.h

        src/history/month_record.cpp
        src/history/month_record.h
        src/history/history_table.h

        src/simulation/simulator.cpp
        src/simulation/simulator.h

        src/events/game_event.cpp
        src/events/game_event.h

        src/telegram/persistent_http.cpp
        src/telegram/persistent_http.h
        src/telegram/tg_bot.cpp
        src/telegram/tg_bot.h
)

target_link_libraries(N_State PRIVATE TgBot::TgBot)


# Тесты
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)

    enable_testing()

    add_executable(N_State_tests

            src/tests/test_main.cpp
            src/tests/test_entity.cpp
            src/tests/test_funding.cpp
            src/tests/test_country.cpp
            src/tests/test_month_record.cpp
            src/tests/test_history_table.cpp
            src/tests/test_simulator.cpp
            src/tests/test_events.cpp
            src/tests/test_game.cpp

            # Исходники классов, которые тестируем
            src/core/entity.cpp
            src/country/funding.cpp
            src/country/country.cpp
            src/history/month_record.cpp
            src/simulation/simulator.cpp
            src/events/game_event.cpp
            src/game/game.cpp
    )

    target_link_libraries(N_State_tests PRIVATE Boost::boost)

    add_test(
            NAME N_State_tests
            COMMAND N_State_tests
    )

endif()