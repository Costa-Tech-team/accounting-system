#include "journal.hpp"
#include "accounting_entry.hpp"
#include "accounts_chart.hpp"

#include <algorithm>

bool isEntryPast(AccountingEntry &past, AccountingEntry &other)
{
    auto pastDate = past.getDate();
    auto otherDate = other.getDate();
    if (pastDate.year() != otherDate.year())
    {
        return pastDate.year() < otherDate.year();
    } else if (pastDate.month() != otherDate.month())
    {
        return pastDate.month() < otherDate.month();
    } else
    {
        return pastDate.day() < otherDate.day();
    }
}

Journal::Journal(std::vector<AccountingEntry> &&entries, AccountsChart &&chart)
  : entries{std::move(entries)},
    chart{std::move(chart)}
{
    std::ranges::sort(entries, isEntryPast);
}

std::span<const AccountingEntry> Journal::getEntries() const { return entries; }
