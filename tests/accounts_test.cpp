#include <stdexcept>
#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <account.hpp>
#include <accounting_entry.hpp>

TEST_CASE("Accountable accounts")
{
    SECTION("Checking contradictory combinations are invalidated")
    {
        auto invalid1 = []() {
            Account("", Account::Category::asset, Account::Currentability::none,
                    1);
        };
        auto invalid2 = []() {
            Account("", Account::Category::netWorth,
                    Account::Currentability::current, 1);
        };

        REQUIRE_THROWS_AS(invalid1(), std::invalid_argument);
        REQUIRE_THROWS_AS(invalid2(), std::invalid_argument);
    }

    SECTION("Getting an account's code")
    {
        Subcategory subcategory{.code = 1,
                                .displayName = "",
                                .category = Account::Category::asset,
                                .currentability =
                                    Account::Currentability::current};
        Superaccount supperaccount{.code = 1, .displayName = ""};
        Account account{"", subcategory, 1, &supperaccount};

        REQUIRE(account.getCode() == "1.1.1.1.1");
    }
}

TEST_CASE("Accountable entries")
{
    SECTION("Creating a entry with differents debit and credit")
    {
        Account acc{"", Account::Category::asset,
                    Account::Currentability::current, 1};
        std::vector<Movement> movements;
        movements.emplace_back(Movement::Type::debit, 3, acc);
        movements.emplace_back(Movement::Type::credit, 1, acc);
        movements.emplace_back(Movement::Type::credit, 1, acc);

        CHECK_THROWS_AS(AccountingEntry(std::move(movements), ""),
                        std::logic_error);
    }

    SECTION("Checking if a entry sorts its movements in order")
    {
        Account ac1{"a", Account::Category::asset,
                    Account::Currentability::current, 1};
        Account ac2{"b", Account::Category::liability,
                    Account::Currentability::current, 3};
        Account ac3{"c", Account::Category::asset,
                    Account::Currentability::current, 2};
        Account ac4{"d", Account::Category::liability,
                    Account::Currentability::current, 4};
        std::vector<Movement> movements;
        movements.emplace_back(Movement::Type::credit, 25, ac4);
        movements.emplace_back(Movement::Type::debit, 25, ac2);
        movements.emplace_back(Movement::Type::credit, 25, ac3);
        movements.emplace_back(Movement::Type::debit, 25, ac1);

        AccountingEntry entry{std::move(movements), ""};
        std::string order;
        for (auto movement : entry.getMovements())
        {
            order += movement.account.get().getDisplayName();
        }

        CHECK(order == "abcd");
    }
}