#include <stdexcept>
#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <account.hpp>

TEST_CASE("Checking contradictory combinations are invalidated")
{
    REQUIRE_THROWS_AS(
        Account(Account::Category::asset, Account::Currentability::none, 1),
        std::invalid_argument);

    REQUIRE_THROWS_AS(Account(Account::Category::netWorth,
                              Account::Currentability::current, 1),
                      std::invalid_argument);
}