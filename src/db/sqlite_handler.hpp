#include <chrono>
#include <filesystem>
#include <memory>

#include <sqlite3.h>

class AccountsChart;
class Journal;

struct SQLite3Deleter
{
    void operator()(sqlite3 *db) const noexcept { sqlite3_close(db); }
};

/// @brief represents a handler for the database that abstracts the system from
/// the database providing a simplified interface for the specific model of the
/// system.
///
/// The database stores accounting entries and accounts in the user's chart.
class SQLiteHandler
{
    using SQLitePointer = std::unique_ptr<sqlite3, SQLite3Deleter>;

  public:
    /// @exception std::runtime_error if unable to create the database
    /// connection.
    /// @param databasePath: if equals ":memory:", it creates the database in
    /// the memory
    SQLiteHandler(const std::filesystem::path &databasePath);
    SQLiteHandler(SQLiteHandler &&) = default;
    SQLiteHandler(const SQLiteHandler &) = delete;
    SQLiteHandler &operator=(SQLiteHandler &&) = default;
    SQLiteHandler &operator=(const SQLiteHandler &) = delete;
    ~SQLiteHandler() = default;

    /// @return a AccountsChart constructed from the accounts in the database.
    [[nodiscard]] AccountsChart retrieveChart() const;

    /// @return a Journal with the entries and the chart stored in the database.
    /// If the chart isn't the one with which the Journal was created it's
    /// probable a failure may occur, but this isn't guaranteed so the user
    /// should make sure this doesn't ever happen.
    /// \todo add a way to enforce a Journal can only be created with a valid
    /// AccountsChart. Maybe with hashing.
    [[nodiscard]] Journal retrieveJournal(AccountsChart &&chart) const;

    /// @return a Journal with only the entries in the date range.
    [[nodiscard]] Journal
    retrieveJournalRange(AccountsChart &&chart,
                         std::chrono::year_month_day start,
                         std::chrono::year_month_day end) const;

    /// @return a Journal with only the entries in the specified month.
    [[nodiscard]] Journal retrieveJournalOnYear(AccountsChart &&chart,
                                                std::chrono::year year) const;

    /// @return a Journal with only the entries in the specified month.
    [[nodiscard]] Journal
    retrieveJournalOnMonth(AccountsChart &&chart,
                           std::chrono::year_month month) const;

  private:
    void execQuery(const char *query);

    void createTables();

    SQLitePointer database;
};
