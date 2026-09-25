#include <chrono>
#include <cstddef>
#include <functional>

#pragma once

class AccountNode;

using Date = std::chrono::year_month_day;

/// @brief represents a single debit or credit in a account. It's part of a
/// accounting entry.
struct Movement
{
  public:
    enum class Type
    {
        debit,
        credit
    };

    Movement(Type type, size_t ammount, const AccountNode &account);

    Type getType() const;
    size_t getAmmount() const;
    const AccountNode &getAccount() const;

  private:
    Type type;
    size_t ammount;
    std::reference_wrapper<const AccountNode> account;
};

/// @brief Represents a entry in a accountable journal for a commercial
/// transaction.
///
/// It stores what accounts are involved in every movement and by how much, but
/// the total of the debit and the credit must be equal. Also the user can store
/// a description or detail for every transaction.
class AccountingEntry
{
  public:
    /// @param movements: the movements that will be moved into the entry.
    /// @param detail: the detail of the entry.
    /// @param date: the date of the entry.
    AccountingEntry(std::vector<Movement> &&movements, std::string_view detail,
                    const Date &date = currentDate());

    AccountingEntry(const AccountingEntry &) = default;
    AccountingEntry(AccountingEntry &&) = default;
    AccountingEntry &operator=(const AccountingEntry &) = default;
    AccountingEntry &operator=(AccountingEntry &&) = default;
    ~AccountingEntry() = default;

    /// @return a view of the movements of the entry, sorted so that all debits
    /// are first and then by the alphabetical order of the accounts.
    std::span<const Movement> getMovements() const;

    /// @return the entry's date.
    Date getDate() const;

  private:
    static Date currentDate();

    std::vector<Movement> movements;
    std::string detail;
    Date date;
};