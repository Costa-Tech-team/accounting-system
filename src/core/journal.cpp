#include "journal.hpp"

#include <algorithm>

#include "accounting_entry.hpp"

static bool isEntryPast(AccountingEntry &past, AccountingEntry &toCompare)
{
    auto pastDate = past.getDate();
    auto otherDate = toCompare.getDate();
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

Journal::Journal(std::vector<AccountingEntry> &&entries, AccountsChart &chart)
  : entries{std::move(entries)},
    chart{chart}
{
    std::ranges::sort(this->entries, isEntryPast);
}

std::span<const AccountingEntry> Journal::getEntries() const { return entries; }

void Journal::addEntry(AccountingEntry &&entry)
{
    entries.emplace_back(std::move(entry));
    std::ranges::sort(this->entries, isEntryPast);
}
