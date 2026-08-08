#include "accounting_entry.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <utility>

static bool alphabeticalOrder(std::string first, std::string last)
{
    std::ranges::transform(first, first.begin(), tolower);
    std::ranges::transform(last, last.begin(), tolower);
    for (unsigned i = 0; i < std::min(first.length(), last.length()); i++)
    {
        if (first[i] != last[i])
        {
            return first[i] < last[i];
        }
    }
    return true;
}

Date AccountingEntry::currentDate()
{
    return std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now());
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
    sortMovements();
}

std::span<const Movement> AccountingEntry::getMovements() const
{
    return movements;
}

void AccountingEntry::sortMovements()
{
    auto movementTypeSorting = [](Movement &lhs,
                                  [[maybe_unused]] Movement &rhs) {
        return lhs.type == Movement::Type::debit;
    };
    std::vector<Movement> sortedMovements;
    std::ranges::sort(movements, movementTypeSorting);
    auto firstCreditIt = std::ranges::find_if(movements, [](Movement m) {
        return m.type == Movement::Type::credit;
    });
    auto alphabeticalSorting = [](Movement &lhs, Movement &rhs) {
        std::string nameFirst{lhs.account.get().getDisplayName()};
        std::string nameLast{rhs.account.get().getDisplayName()};
        return alphabeticalOrder(nameFirst, nameLast);
    };
    std::sort(movements.begin(), firstCreditIt, alphabeticalSorting);
    std::sort(firstCreditIt, movements.end(), alphabeticalSorting);
}

Date AccountingEntry::getDate() const { return date; }
