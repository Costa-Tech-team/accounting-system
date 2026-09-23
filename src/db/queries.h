namespace queries
{
namespace schema
{
const auto setForeingKeys = "PRAGMA foreign_keys = true;";

const auto createIndexes =
    R"(CREATE INDEX IF NOT EXISTS idx_movements_entry ON 
    journal_entry_movements(journal_entry_id);
    CREATE INDEX IF NOT EXISTS idx_movements_account ON 
    journal_entry_movements(account_id);)";

const auto createAccountsTable =
    R"(CREATE TABLE IF NOT EXISTS accounts (
        id INTEGER PRIMARY KEY NOT NULL,
        account_category_code INTEGER NOT NULL,
        currentability_code INTEGER,
        parent_id INTEGER,
        code TEXT NOT NULL UNIQUE,
        name TEXT NOT NULL UNIQUE,
        is_operable INTEGER DEFAULT TRUE NOT NULL,
        is_root INTEGER GENERATED ALWAYS AS (parent_id IS NULL) VIRTUAL,
        FOREIGN KEY (parent_id) REFERENCES accounts(id)
        CHECK(account_category_code >= 0 AND account_type_code <= 5)
        CHECK(currentability_code IN (0, 1))
        CHECK(parent_id != id)
        CHECK(is_operable IN (0, 1))
    ) STRICT;)";

const auto createEntriesTable =
    R"(CREATE TABLE IF NOT EXISTS journal_entries ( 
        id INTEGER PRIMARY KEY NOT NULL,
        entry_date TEXT NOT NULL,
        detail TEXT,
        CHECK(date(entry_date) IS NOT NULL)
    ) STRICT;)";

const auto createMovementsTable =
    R"(CREATE TABLE IF NOT EXISTS journal_entry_movements ( 
        id INTEGER PRIMARY KEY NOT NULL,
        journal_entry_id INTEGER NOT NULL,
        account_id INTEGER NOT NULL,
        debit INTEGER DEFAULT 0 NOT NULL,
        credit INTEGER DEFAULT 0 NOT NULL,
        FOREIGN KEY (journal_entry_id) REFERENCES journal_entries(id),
        FOREIGN KEY (account_id) REFERENCES accounts(id)
        CHECK(debit >= 0),
        CHECK(credit >= 0),
        CHECK((debit == 0) != (credit == 0))
    ) STRICT;)";

const auto tablesCreated =
    R"(SELECT COUNT(*) FROM sqlite_master 
    WHERE type = 'table' 
    AND name IN 
    ('account_types', 'accounts', 'journal_entries', 'journal_entry_movements');)";

} // namespace schema

namespace inserts
{
const auto account =
    R"(INSERT INTO accounts(account_type_code, parent_id, name, code, is_operable)
    VALUES(?, ?, ?, ?);)";
}

namespace select
{

/// Use these enums when selecting columns with SQLite, which uses indexes for
/// columns starting from 0. Put using statements at function level, never at
/// the global level.
namespace RootAccounts
{
const auto sql =
    "SELECT id, category, currentability, parent_id, code, name, postable, "
    "root FROM accounts WHERE is_root = TRUE ORDER BY code";
enum Indexes
{
    column_id = 1,
    column_category,
    column_currentability,
    column_parentID,
    column_code,
    column_name,
    column_postable,
    column_root
};
} // namespace RootAccounts

namespace ChildrenOfAccount
{
const auto sql = "SELECT id, name, postable FROM accounts WHERE parent_id = "
                 "? ORDER BY code";

enum Indexes
{
    column_id,
    column_name,
    column_postable,
};

} // namespace ChildrenOfAccount

} // namespace select

} // namespace queries