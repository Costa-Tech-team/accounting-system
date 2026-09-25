#include <chrono>
#include <functional>
#include <span>
#include <vector>

#include "accounts_chart.hpp"

#pragma once

class AccountingEntry;

/// @brief represents an accounting journal, holding informaiton about the
/// accounts chart and the entries. One instance holds only one accounting
/// year at once.
class Journal
{
  public:
    /// @param entries if not in chronological order, the constructor will
    /// reorder these.
    /// @param chart a accounts chart that will be owned by the journal.
    Journal(std::vector<AccountingEntry> &&entries, AccountsChart &chart,
            std::chrono::year journalYear);

    // void exportToFile(std::filesystem::path path);
    /// \todo implement accounting journal export to a sheet file.

    /// @return a view of the entries in chronological order.
    std::span<AccountingEntry> getEntries();
    /// @return a const view of the entries in chronological order.
    std::span<const AccountingEntry> getEntries() const;

    void addEntry(AccountingEntry &&entry);

    /// @return a reference to the AccountsChart
    AccountsChart &getChart();
    /// @return a const reference to the AccountsChart
    const AccountsChart &getChart() const;

  private:
    std::chrono::year journalYear;
    std::vector<AccountingEntry> entries;
    std::reference_wrapper<AccountsChart> chart;
};