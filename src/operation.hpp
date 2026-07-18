#include <chrono>

#include "account.hpp"

using Date = std::chrono::year_month_day;

/// @brief Represents a single entry or operation in the book.
class Operation
{
  public:
    enum class Type
    {
        debit,
        credit
    };

    Operation(double amount, const Account &account,
              const Date &date = {currentDate()});

    /// @return the account to which the operation belongs to.
    Account getAccount() const;

    /// @return wether the operation is a debit or a credit one.
    Type getType() const;

    /// @return the date on which the operation was made.
    Date getDate() const;

    /// @return how much money in debited or credited in the operation.
    double getAmount() const;

  private:
    static Date currentDate();

    Account account;
    Type type;
    Date date;
    double amount;
};