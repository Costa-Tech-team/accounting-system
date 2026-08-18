#include "accounts_chart.hpp"
#include "account_node.hpp"
#include <algorithm>
#include <memory>

AccountsChart::AccountsChart(AccountsTree &&accountsTree)
  : accountsTree{std::move(accountsTree)}
{}

AccountNode *AccountsChart::getByCode(std::string_view code)
{
    auto element = std::ranges::find_if(
        accountsTree,
        [code](
            AccountsTree::bidirectional_iterator::reference account) -> bool {
            return account.getCode() == code;
        });
    return element != accountsTree.end() ? std::to_address(element) : nullptr;
}

const AccountNode *AccountsChart::getByCode(std::string_view code) const
{
    auto element = std::ranges::find_if(
        accountsTree,
        [code](AccountsTree::const_bidirectional_iterator::reference account)
            -> bool {
            return account.getCode() == code;
        });
    return element != accountsTree.end() ? std::to_address(element) : nullptr;
}
