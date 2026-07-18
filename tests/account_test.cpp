#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <account.hpp>

TEST_CASE("Checking contradictory combinations are invalidated")
{
    REQUIRE_FALSE(isValid(Account::Category::assetOrLiability));

    REQUIRE_FALSE(isValid(Account::Category::revenueOrExpense));

    REQUIRE_FALSE(
        isValid(Account::Category::asset | Account::Category::revenue));

    REQUIRE_FALSE(
        isValid(Account::Category::netWorth | Account::Category::asset));

    REQUIRE_FALSE(
        isValid(Account::Category::netWorth | Account::Category::revenue));
}