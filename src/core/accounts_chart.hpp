#include "account_node.hpp"

#include "../tree.hpp"

#pragma once

using AccountsTree = Tree<AccountNode>;

static_assert(TreeNode<AccountNode>);

/// @brief Represents an accounts chart for a organization that ismodifiable.
/// common use accounts are pre-defined.
///
/// \todo define somewhere default accounts
class AccountsChart
{
  public:
    AccountsChart(AccountsTree &&accountsTree);

    /// @return the account with the code if present, null otherwise.
    AccountNode *getByCode(std::string_view code);
    /// \overload
    const AccountNode *getByCode(std::string_view code) const;

  private:
    AccountsTree accountsTree;
};