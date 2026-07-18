#include <cstdint>
#include <string>
#include <string_view>

/// @brief This class contains information about an accountable account.
class Account
{
  public:
    /// This enum has the categories of accountable accounts, the last
    /// two items are provided for convenience when checking if a account has
    /// one of two mutually exclusives flags.
    enum class Category : uint8_t
    {
        none = 0,
        asset = 1,
        liability = 1 << 1,
        revenue = 1 << 2,
        expense = 1 << 3,
        netWorth = 1 << 4,

        assetOrLiability = 0b11,
        revenueOrExpense = 0b11 << 2,
    };

    /// @param acccountName: should be provided by an account chart.
    /// @param category: it asserts if it's invalid (see
    /// isValid(Account::Category category)).
    Account(Category category, std::string_view accountName);

    /// @return the underlying account category.
    Category getAccountCategory() const;

    /// @return the name of the account.
    std::string getAccountName() const;

  private:
    Category accountCategory;
    std::string accountName;
};

/// \related Account
/// @param category: the item to check
/// @param toFullfil: the category to check wether category is a part of
///
/// @return true if there at least one flag present both in category and in
/// toFullfil
bool fullfilsCategory(Account::Category category, Account::Category toFullfil);

/// \related Account
/// @return true if the category is either an asset, a liability, a revenue, a
/// expense, or net worth, without any combination of these five
bool isValid(Account::Category category);

/// @return the combination of both categories' flags.
Account::Category operator|(Account::Category lhs, Account::Category rhs);

/// @return the intersection of both categories' flags.
Account::Category operator&(Account::Category lhs, Account::Category rhs);