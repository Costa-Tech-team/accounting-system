#include "accounts_chart.hpp"
#include <vector>

std::span<const AccountNode> AccountsChart::getAccounts() const
{
    return topLevelAccounts;
}
