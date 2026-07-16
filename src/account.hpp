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
        current = 1 << 4,
        fixed = 1 << 5,
        debit = 1 << 6,
        credit = 1 << 7
    };

    Account(Category category, std::string_view accountName);

    std::string toString();

    bool accomplishesCategory(Category category);

  private:
    Category accountCategory;
    std::string accountName;
};

Account::Category operator|(Account::Category lhs, Account::Category rhs);

Account::Category operator&(Account::Category lhs, Account::Category rhs);