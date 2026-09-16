#include "database_handler.hpp"

#include <cstring>

#include <sqlite3.h>

#include "queries.h"

DatabaseHandler::DatabaseHandler(const std::filesystem::path &databasePath)
{
    sqlite3 *tmp;
    sqlite3_open(databasePath.c_str(), &tmp);
    database.reset(tmp);
}

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
