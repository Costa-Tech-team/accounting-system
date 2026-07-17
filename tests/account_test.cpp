#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <account.hpp>

TEST_CASE("Checking contradictory combinations are invalidated")
{
    auto invalid = Account::Category::asset | Account::Category::liability |
                   Account::Category::debit;
    REQUIRE_FALSE(isValid(invalid));

    invalid = Account::Category::revenue | Account::Category::expense |
              Account::Category::debit;
    REQUIRE_FALSE(isValid(invalid));

    invalid = Account::Category::asset | Account::Category::debit |
              Account::Category::credit;
    REQUIRE_FALSE(isValid(invalid));
}