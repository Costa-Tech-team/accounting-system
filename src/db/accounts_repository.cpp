#include "accounts_repository.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <memory>
#include <sqlite3.h>
#include <string>
#include <vector>

// #include "../core/accounts_chart.hpp"
#include "queries.h"

using AccountIDPair = std::pair<AccountNode, int>;

static std::string columnString(sqlite3_stmt *stmt, int index)
{
    auto text = sqlite3_column_text(stmt, index);
    int length = sqlite3_column_bytes(stmt, index);
    auto textFixed = std::bit_cast<const char *, const unsigned char *>(text);
    return {textFixed, static_cast<size_t>(length)};
}

static uint8_t accountSubcode(sqlite3_stmt *stmt)
{
    using namespace queries::select::RootAccounts;

    int dotsToSkip = 1;
    dotsToSkip += sqlite3_column_int(stmt, column_currentability) == 0;
    std::string fullCode{columnString(stmt, column_code)};
    std::string subCode{fullCode};
    for (int i = 0; i < dotsToSkip; i++)
    {
        auto dotPosition = std::ranges::find(fullCode, '.');
        subCode = std::string(std::to_address(dotPosition++),
                              std::to_address(fullCode.end()));
    }
    auto dotPosition = std::ranges::find(fullCode, '.');
    subCode = std::string(subCode.data(), std::to_address(dotPosition));
    return std::stoi(subCode);
}

AccountsRepository::AccountsRepository(sqlite3 *database)
{
    sqlite3_stmt *tmp;
    sqlite3_prepare_v2(database, queries::inserts::account, -1, &tmp, nullptr);
    insertAccount.reset(tmp);
    sqlite3_prepare_v2(database, queries::select::RootAccounts::sql, -1, &tmp,
                       nullptr);
    selectRootAccounts.reset(tmp);
}

// AccountsChart AccountsRepository::retrieveChart() const
// {
//     auto accountPairs = fillFromTable();
// }

void AccountsRepository::throwSQLerror(int code, const char *sql) const
{
    throw std::runtime_error(
        "The following SQL query finalized with the error code " +
        std::to_string(code) + "\n\n" + sql);
}

std::vector<AccountIDPair> AccountsRepository::fillFromTable() const
{
    int stepResult = sqlite3_step(selectRootAccounts.get());
    std::vector<AccountIDPair> accounts;
    while (stepResult == SQLITE_ROW)
    {
        accounts.emplace_back(createRootStep());
        stepResult = sqlite3_step(selectRootAccounts.get());
    }
    if (stepResult != SQLITE_DONE)
    {
        throwSQLerror(stepResult, queries::select::RootAccounts::sql);
    }
    sqlite3_reset(selectRootAccounts.get());
    for (auto &rootAccount : accounts)
    {
        createChildren(rootAccount.first, rootAccount.second);
    }

    return accounts;
}

AccountIDPair AccountsRepository::createRootStep() const
{
    using namespace queries::select::RootAccounts;

    auto stmt = selectRootAccounts.get();
    assert(sqlite3_column_int(stmt, column_root) == true);
    auto name = columnString(stmt, column_name);
    uint8_t subcode = accountSubcode(selectRootAccounts.get());
    auto category = static_cast<AccountNode::Category>(
        sqlite3_column_int(stmt, column_category));
    auto currentability = static_cast<AccountNode::Currentability>(
        sqlite3_column_int(stmt, column_currentability));
    bool postable = sqlite3_column_int(stmt, column_postable);
    AccountNode account{name, subcode, category, currentability, postable};
    return {std::move(account), sqlite3_column_int(stmt, 1)};
}

void AccountsRepository::createChildren(AccountNode &account, int id) const
{
    auto stmt = selectAccountChildren.get();
    sqlite3_bind_int(stmt, 1, id);
    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_DONE)
    {
        return;
    }
    while (stepResult == SQLITE_ROW)
    {
        int childID = createChildStep(account);
        createChildren(account.getChildren().back(), childID);
        stepResult = sqlite3_step(stmt);
    }
    if (stepResult != SQLITE_DONE)
    {
        throwSQLerror(stepResult, queries::select::ChildrenOfAccount::sql);
    }
}

int AccountsRepository::createChildStep(AccountNode &parent) const
{
    using namespace queries::select::ChildrenOfAccount;

    auto stmt = selectAccountChildren.get();
    int childID = sqlite3_column_int(stmt, column_id);
    std::string name = columnString(selectAccountChildren.get(), column_name);
    bool postable = sqlite3_column_int(stmt, column_postable);
    parent.addChild(AccountNode(name, parent, postable));
    return childID;
}

void AccountsRepository::StatementDeleter::operator()(
    sqlite3_stmt *stmt) const noexcept
{
    sqlite3_finalize(stmt);
}
