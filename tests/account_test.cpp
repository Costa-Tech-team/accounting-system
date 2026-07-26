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

TEST_CASE("Getting an account's code")
{
    Subcategory subcategory{.code = 1, .displayName = ""};
    Account account{Account::Category::asset, Account::Currentability::current,
                    1, &subcategory, 1};
    REQUIRE(account.getCode() == "1.1.1.1.1");
}