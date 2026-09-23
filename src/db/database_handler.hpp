#include <chrono>
#include <filesystem>
#include <memory>

#include <sqlite3.h>

class AccountsChart;
class Journal;
class AccountingEntry;
class AccountNode;

/// @brief Represents a handler for the database that abstracts the system
/// from the database providing a simplified interface for the specific
/// model of the system.
///
/// The database stores accounting entries with their movements and accounts in
/// the user's chart. The underlying database is SQLite.
class DatabaseHandler
{
  public:
    /// @exception std::runtime_error if unable to create the database
    /// connection.
    /// @param databasePath: if equals ":memory:", it creates the database in
    /// the memory
    DatabaseHandler(const std::filesystem::path &databasePath);
    DatabaseHandler(DatabaseHandler &&) = default;
    DatabaseHandler(const DatabaseHandler &) = delete;
    DatabaseHandler &operator=(DatabaseHandler &&) = default;
    DatabaseHandler &operator=(const DatabaseHandler &) = delete;
    ~DatabaseHandler() = default;

    /// @return a AccountsChart constructed from the accounts in the database.
    [[nodiscard]] AccountsChart retrieveChart() const;

    /// @return a Journal with the entries and the chart stored in the database.
    /// If the chart isn't the one with which the Journal was created it's
    /// probable a failure may occur, but this isn't guaranteed so the user
    /// should make sure this doesn't ever happen.
    /// \todo add a way to enforce a Journal can only be created with a valid
    /// AccountsChart. Maybe with hashing.
    [[nodiscard]] Journal retrieveJournal(AccountsChart &&chart) const;

    /// @return a Journal with only the entries in the specified year's
    /// excercise.
    [[nodiscard]] Journal retrieveJournalOnYear(AccountsChart &&chart,
                                                std::chrono::year year) const;

    /// @return a Journal with only the entries in the date range.
    [[nodiscard]] Journal
    retrieveJournalRange(AccountsChart &&chart,
                         std::chrono::year_month_day start,
                         std::chrono::year_month_day end) const;

    /// @return a Journal with only the entries in the specified month.
    [[nodiscard]] Journal
    retrieveJournalOnMonth(AccountsChart &&chart,
                           std::chrono::year_month month) const;

    /// Saves the entries and the chart on the database.
    void saveJournal(const Journal &journal);

  private:
    void execQuery(const char *query);

    void createTables();

    struct SQLite3Deleter
    {
        void operator()(sqlite3 *db) const noexcept;
    };

    struct StatementDeleter
    {
        void operator()(sqlite3_stmt *stmt) const noexcept;
    };

    using StmtPointer = std::unique_ptr<sqlite3_stmt, StatementDeleter>;

    std::unique_ptr<sqlite3, SQLite3Deleter> database;
};
