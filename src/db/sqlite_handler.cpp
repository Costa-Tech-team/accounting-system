#include "sqlite_handler.hpp"

#include <cstring>

#include <sqlite3.h>

#include "../core/accounts_chart.hpp"
#include "../core/journal.hpp"
#include "queries.h"

struct StatementDeleter
{
    void operator()(sqlite3_stmt *stmt) const noexcept
    {
        sqlite3_finalize(stmt);
    }
};

SQLiteHandler::SQLiteHandler(const std::filesystem::path &databasePath)
{
    sqlite3 *tmp;
    sqlite3_open(databasePath.c_str(), &tmp);
    database.reset(tmp);
}

void SQLiteHandler::execQuery(const char *query)
{
    sqlite3_exec(database.get(), query, nullptr, nullptr, nullptr);
}

void SQLiteHandler::createTables()
{
    execQuery(queries::setForeingKeys);
    execQuery(queries::createAccountsTable);
    execQuery(queries::createAccountTypesTable);
    execQuery(queries::createEntriesTable);
    execQuery(queries::createMovementsTable);
}
