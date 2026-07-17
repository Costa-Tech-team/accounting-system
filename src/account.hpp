#include <cstdint>
#include <string>
#include <string_view>

class Account
{
  public:
    enum class Category : uint8_t
    {
        none = 0,
        asset = 1 << 0,
        liability = 1 << 1,
        revenue = 1 << 2,
        expense = 1 << 3,
        debit = 1 << 6,
        credit = 1 << 7,

        assetOrLiability = 0b11,
        revenueOrExpense = 0b11 << 2,
        debitOrCredit = 0b11 << 6,
    };

    Account(Category category, std::string_view accountName);

    Category getAccountCategory();

    std::string toString();

  private:
    Category accountCategory;
    std::string accountName;
};

bool accomplishesCategory(Account::Category category,
                          Account::Category toAccomplish);

bool isValid(Account::Category category);

Account::Category operator|(Account::Category lhs, Account::Category rhs);

Account::Category operator&(Account::Category lhs, Account::Category rhs);