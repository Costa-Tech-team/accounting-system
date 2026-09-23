#include "database_handler.hpp"

#include <cstring>

#include <sqlite3.h>
#include <stdexcept>
#include <string>

#include "queries.h"

DatabaseHandler::DatabaseHandler(const std::filesystem::path &databasePath)
{
    sqlite3 *tmp;
    int result = sqlite3_open(databasePath.c_str(), &tmp);
    database.reset(tmp);
    if (result != SQLITE_OK)
    {
        throw std::runtime_error("Database connection failed with error code " +
                                 std::to_string(result));
    }
}

// AccountsChart DatabaseHandler::retrieveChart() const {}

// Journal DatabaseHandler::retrieveJournalOnYear(AccountsChart &&chart,
//                                                std::chrono::year year) const
// {}

// Journal
// DatabaseHandler::retrieveJournalRange(AccountsChart &&chart,
//                                       std::chrono::year_month_day start,
//                                       std::chrono::year_month_day end) const
// {}

// Journal
// DatabaseHandler::retrieveJournalOnMonth(AccountsChart &&chart,
//                                         std::chrono::year_month month) const
// {}

// void DatabaseHandler::saveJournal(const Journal &journal) {}

void DatabaseHandler::execQuery(const char *query)
{
    sqlite3_exec(database.get(), query, nullptr, nullptr, nullptr);
}

void DatabaseHandler::createTables()
{
    execQuery(queries::schema::setForeingKeys);
    execQuery(queries::schema::createAccountsTable);
    execQuery(queries::schema::createEntriesTable);
    execQuery(queries::schema::createMovementsTable);
    execQuery(queries::schema::createIndexes);
}

void DatabaseHandler::SQLite3Deleter::operator()(sqlite3 *db) const noexcept
{
    sqlite3_close(db);
}
void DatabaseHandler::StatementDeleter::operator()(
    sqlite3_stmt *stmt) const noexcept
{
    sqlite3_finalize(stmt);
}
