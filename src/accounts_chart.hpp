#include <span>
#include <vector>

#include "account.hpp"

/// @brief Represents an accounts chart for a organization that is modifiable.
/// common use accounts are pre-defined.
///
/// \todo define somewhere default accounts
class AccountsChart
{
  public:
    AccountsChart();

    std::span<const Subcategory> getSubcategories() const;
    std::span<const Superaccount> getSuperaccounts() const;
    std::span<const Account> getAccounts() const;

    std::vector<const Account *>
    filterByCurrentability(Account::Category category,
                           Account::Currentability currentability);

    std::vector<const Account *> filterBySubcategory();

  private:
    std::vector<Subcategory> subcategories;
    std::vector<Superaccount> superaccount;
    std::vector<Account> accounts;
};