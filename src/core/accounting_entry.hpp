#include <chrono>
#include <functional>

#include "account.hpp"

#pragma once

using Date = std::chrono::year_month_day;

/// @brief represents a single debit or credit in a account. It's part of a
/// accounting entry.
struct Movement
{
    enum class Type
    {
        debit,
        credit
    };

    Type type;
    int ammount;
    std::reference_wrapper<Account> account;
};

/// @brief Represents a entry in a accountable journal for a commercial
/// transaction. It stores what accounts are involved and by how much, but the
/// total of the debit and the credit must be equal. Also the user can store a
/// description or detail for every transaction.
class AccountingEntry
{
  public:
    AccountingEntry(std::vector<Movement> &&movements, std::string_view detail,
                    const Date &date = currentDate());

    /// @return a view of the movements of the entry, sorted so that every debit
    /// are first and then by the alphabetical order of the accounts.
    std::span<const Movement> getMovements() const;

    /// @return the date on which the transaction was made.
    Date getDate() const;

  private:
    static Date currentDate();

    void sortMovements();

    std::vector<Movement> movements;
    std::string detail;
    Date date;
};