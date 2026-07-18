#include <cstdint>
#include <string>
#include <string_view>

/// @brief This class contains information about an accountable account.
class Account
{
  public:
    /// This enum has the categories of accountable accounts, the last
    /// three items are provided for convenience when checking if a account has
    /// one of two mutually exclusives flags.
    enum class Category : uint8_t
    {
        none = 0,
        asset = 1,
        liability = 1 << 1,
        revenue = 1 << 2,
        expense = 1 << 3,
        debit = 1 << 4,
        credit = 1 << 5,

        assetOrLiability = 0b11,
        revenueOrExpense = 0b11 << 2,
        debitOrCredit = 0b11 << 4,
    };

    /// constructor, the acccount name should be provided by an account
    /// chart. It asserts if the category is invalid (see
    /// isValid(ccount::Category category)).
    Account(Category category, std::string_view accountName);

    /// @return the underlying account category.
    Category getAccountCategory();

    /// @return a string representation of the account, with its name and a sign
    /// "+" or "-" between parenthesis depending on wether the account is adebit
    /// or a credit.
    std::string toString();

  private:
    Category accountCategory;
    std::string accountName;
};

/// \related Account
/// @param category: the item to check
/// @param toAccomplish: the category to check wether category is a part of
///
/// @return true if there at least one flag present both in category and in
/// toFullfil
bool fullfilsCategory(Account::Category category, Account::Category toFullfil);

/// \related Account
/// @return true if the following conditions are true:
/// - The category is either an asset, a liability, a revenue or a expense,
/// without combination of these four
/// - The category is a debit or a credit
bool isValid(Account::Category category);

/// @return the combination of both categories' flags.
Account::Category operator|(Account::Category lhs, Account::Category rhs);

/// @return the intersection of both categories' flags.
Account::Category operator&(Account::Category lhs, Account::Category rhs);