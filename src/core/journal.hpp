#include <span>
#include <vector>

#include "accounting_entry.hpp"
#include "accounts_chart.hpp"

#pragma once

/// @brief represents an accounting journal, holding informaiton about the
/// accounts chart and the entries.
class Journal
{
  public:
    /// @param entries if not in chronological order, the constructor will
    /// reorder these.
    Journal(std::vector<AccountingEntry> &&entries, AccountsChart &&chart);

    // void exportToFile(std::filesystem::path path);
    /// \todo implement accounting journal export to a sheet file.

    /// @return a view of the entries in chronological order.
    std::span<const AccountingEntry> getEntries() const;

  private:
    std::vector<AccountingEntry> entries;
    AccountsChart chart;
};