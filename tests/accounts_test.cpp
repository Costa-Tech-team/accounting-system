#include <stdexcept>
#include <string>
#define CATCH_CONFIG_MAIN

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <core/account_node.hpp>
#include <core/accounting_entry.hpp>

TEST_CASE("Accountable accounts")
{
    SECTION("Checking contradictory combinations are invalidated")
    {
        auto invalid1 = []() {
            AccountNode("", 1, AccountNode::Category::asset,
                        AccountNode::Currentability::none);
        };
        auto invalid2 = []() {
            AccountNode("", 1, AccountNode::Category::netWorth,
                        AccountNode::Currentability::current);
        };

        REQUIRE_THROWS_AS(invalid1(), std::invalid_argument);
        REQUIRE_THROWS_AS(invalid2(), std::invalid_argument);
    }

    SECTION("Getting an account's code")
    {

        AccountNode parentAccount{"", 1, AccountNode::Category::asset,
                                  AccountNode::Currentability::current};
        AccountNode childAccount{"", parentAccount};
        REQUIRE(parentAccount.getCode() == "1.1.1");
        REQUIRE(childAccount.getCode() == "1.1.1.1");
    }
}

TEST_CASE("Accountable entries")
{
    SECTION("Creating a entry with differents debit and credit")
    {
        AccountNode acc{"", 1, AccountNode::Category::asset,
                        AccountNode::Currentability::current, 1};
        std::vector<Movement> movements;
        movements.emplace_back(Movement::Type::debit, 3, acc);
        movements.emplace_back(Movement::Type::credit, 1, acc);
        movements.emplace_back(Movement::Type::credit, 1, acc);

        CHECK_THROWS_AS(AccountingEntry(std::move(movements), ""),
                        std::logic_error);
    }

    SECTION("Checking if a entry sorts its movements in order")
    {
        AccountNode ac1{"a", 1, AccountNode::Category::asset,
                        AccountNode::Currentability::current, true};
        AccountNode ac2{"b", 2, AccountNode::Category::liability,
                        AccountNode::Currentability::current, true};
        AccountNode ac3{"c", 3, AccountNode::Category::asset,
                        AccountNode::Currentability::current, true};
        AccountNode ac4{"d", 4, AccountNode::Category::liability,
                        AccountNode::Currentability::current, true};
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