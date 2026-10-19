#include <iostream>

void test_entity();
void test_funding();
void test_country();
void test_month_record();
void test_history_table();
void test_simulator();
void test_events();
void test_game();

int main()
{
    std::cout << "Country N тесты\n\n";

    std::cout << "1/8 Entity\n";
    test_entity();

    std::cout << "2/8 FundingTable\n";
    test_funding();

    std::cout << "3/8 Country\n";
    test_country();

    std::cout << "4/8 MonthRecord\n";
    test_month_record();

    std::cout << "5/8 HistoryTable\n";
    test_history_table();

    std::cout << "6/8 ISimulator / MonthSimulator\n";
    test_simulator();

    std::cout << "7/8 GameEvent / Events\n";
    test_events();

    std::cout << "8/8 Game\n";
    test_game();

    std::cout << "\nвсе тесты пройдены!\n";

    return 0;
}