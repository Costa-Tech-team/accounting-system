#include "accounting_entry.hpp"

#include <algorithm>
#include <functional>
#include <utility>

#include "account_node.hpp"

Movement::Movement(Type type, size_t ammount, const AccountNode &account)
  : type{type},
    ammount{ammount},
    account{std::cref(account)}
{}

Movement::Type Movement::getType() const { return type; }

size_t Movement::getAmmount() const { return ammount; }

const AccountNode &Movement::getAccount() const { return account.get(); }

static bool alphabeticalOrder(Movement &lhs, Movement &rhs)
{
    std::string nameFirst{lhs.getAccount().getDisplayName()};
    std::string nameLast{rhs.getAccount().getDisplayName()};
    std::ranges::transform(nameFirst, nameFirst.begin(), tolower);
    std::ranges::transform(nameLast, nameLast.begin(), tolower);
    auto minLength = std::min(nameFirst.length(), nameLast.length());
    for (unsigned i = 0; i < minLength; i++)
    {
        if (nameFirst[i] != nameLast[i])
        {
            return nameFirst[i] < nameLast[i];
        }
    }
    return true;
}

static bool sortMovementsPredicate(Movement &lhs, Movement &rhs)
{
    if (lhs.getType() == rhs.getType())
    {
        return alphabeticalOrder(lhs, rhs);
    } else
    {
        return lhs.getType() == Movement::Type::debit;
    }
}

AccountingEntry::AccountingEntry(std::vector<Movement> &&movements,
                                 std::string_view detail, const Date &date)
  : movements{std::move(movements)},
    detail{detail},
    date{date}
{
    int totalDebit = 0, totalCredit = 0;
    for (const Movement &entry : this->movements)
    {
        entry.getType() == Movement::Type::debit ?
            totalDebit += entry.getAmmount() :
            totalCredit += entry.getAmmount();
    }
    if (totalDebit != totalCredit)
    {
        throw std::logic_error(
            "Total debit must be equal to total credit in accounting entries");
    }
    std::ranges::sort(this->movements, sortMovementsPredicate);
}

std::span<const Movement> AccountingEntry::getMovements() const
{
    return movements;
}

Date AccountingEntry::getDate() const { return date; }

Date AccountingEntry::currentDate()
{
    return std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now());
}
