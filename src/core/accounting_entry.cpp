#include "accounting_entry.hpp"

#include <algorithm>
#include <utility>

#include "account_node.hpp"

static bool alphabeticalOrder(Movement &lhs, Movement &rhs)
{
    std::string nameFirst{lhs.account.get().getDisplayName()};
    std::string nameLast{rhs.account.get().getDisplayName()};
    std::ranges::transform(nameFirst, nameFirst.begin(), tolower);
    std::ranges::transform(nameLast, nameLast.begin(), tolower);
    for (unsigned i = 0; i < std::min(nameFirst.length(), nameLast.length());
         i++)
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
    if (lhs.type == rhs.type)
    {
        return alphabeticalOrder(lhs, rhs);
    } else
    {
        return lhs.type == Movement::Type::debit;
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
        entry.type == Movement::Type::debit ? totalDebit += entry.ammount :
                                              totalCredit += entry.ammount;
    }
    if (totalDebit != totalCredit)
    {
        throw std::logic_error("Debit must be equal to credit");
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
