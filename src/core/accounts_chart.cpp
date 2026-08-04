#include "accounts_chart.hpp"
#include <vector>

std::span<const Subcategory> AccountsChart::getSubcategories() const
{
    return subcategories;
}

std::span<const Superaccount> AccountsChart::getSuperaccounts() const
{
    return superaccount;
}

std::span<const Account> AccountsChart::getAccounts() const { return accounts; }

std::vector<const Account *>
AccountsChart::filterByCurrentability(Account::Category category,
                                      Account::Currentability currentability)
{
    std::vector<const Account *> result;
    for (auto &account : accounts)
    {
        if (account.getCategory() == category &&
            account.getCurrentability() == currentability)
        {
            result.push_back(&account);
        }
    }
    return result;
}