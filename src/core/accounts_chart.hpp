#include <span>
#include <vector>

#include "account_node.hpp"

#pragma once

/// @brief Represents an accounts chart for a organization that is modifiable.
/// common use accounts are pre-defined.
///
/// \todo define somewhere default accounts
class AccountsChart
{
  public:
    AccountsChart();

    std::span<const AccountNode> getAccounts() const;

  private:
    std::vector<AccountNode> topLevelAccounts;
};