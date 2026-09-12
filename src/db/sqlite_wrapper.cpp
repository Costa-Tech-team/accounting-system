#include "sqlite_wrapper.hpp"

#include "sqlite3.h"

sqlite3_stmt *Statement::getHandle() { return statement; }

Statement::Statement(sqlite3 *database, std::string_view sql)
{
    sqlite3_prepare_v2(database, sql.data(), sql.size(), &statement, nullptr);
}

Statement::Statement(Statement &&other) noexcept
{
    if (this != &other)
    {
        if (statement)
        {
            sqlite3_finalize(statement);
        }
        statement = other.statement;
        other.statement = nullptr;
    }
}

Statement &Statement::operator=(Statement &&other) noexcept
{
    if (this != &other)
    {
        if (statement)
        {
            sqlite3_finalize(statement);
        }
        statement = other.statement;
        other.statement = nullptr;
    }
    return *this;
}

Statement::~Statement()
{
    sqlite3_finalize(statement);
    statement = nullptr;
}

void Statement::bind(int index, int64_t value)
{
    sqlite3_bind_int64(statement, index, value);
}

void Statement::bind(int index, double value)
{
    sqlite3_bind_double(statement, index, value);
}

void Statement::bind(int index, std::string_view value)
{
    sqlite3_bind_text(statement, index, value.data(), value.size(),
                      SQLITE_STATIC);
}

void Statement::bindNull(int index) { sqlite3_bind_null(statement, index); }

bool Statement::step() { return sqlite3_step(statement) == SQLITE_DONE; }

void Statement::reset() { sqlite3_reset(statement); }

void Statement::clear() { sqlite3_clear_bindings(statement); }

SQLite3 Statement::databaseHandle() { return {sqlite3_db_handle(statement)}; }

bool Statement::readOnly() const { return sqlite3_stmt_readonly(statement); }

int64_t Statement::columnInt64(int index) const
{
    return sqlite3_column_int64(statement, index);
}

double Statement::columnDouble(int index) const
{
    return sqlite3_column_double(statement, index);
}

std::string Statement::columnText(int index) const
{
    auto data = sqlite3_column_text(statement, index);
    int bytes = sqlite3_column_bytes(statement, index);
    return {data, std::next(data, bytes)};
}

SQLite3::SQLite3(const std::filesystem::path &databasePath)
{
    int result = sqlite3_open(databasePath.c_str(), &database);
    if (result != SQLITE_OK)
    {
        throw std::runtime_error(
            "SQLite database connection failed with error code " +
            std::to_string(result));
    }
}

SQLite3::SQLite3(SQLite3 &&other) noexcept
{
    if (this != &other)
    {
        if (database)
        {
            sqlite3_close(database);
        }
        database = other.database;
        other.database = nullptr;
    }
}

SQLite3 &SQLite3::operator=(SQLite3 &&other) noexcept
{
    if (this != &other)
    {
        if (database)
        {
            sqlite3_close(database);
        }
        database = other.database;
        other.database = nullptr;
    }
    return *this;
}

void SQLite3::exec(std::string_view sql)
{
    sqlite3_exec(database, sql.data(), nullptr, nullptr, nullptr);
}

Statement SQLite3::prepare(std::string_view sql) { return {database, sql}; }
