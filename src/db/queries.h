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
        account_type_code INTEGER NOT NULL,
        parent_id INTEGER,
        code TEXT NOT NULL UNIQUE,
        name TEXT NOT NULL UNIQUE,
        is_operable BOOLEAN DEFAULT TRUE NOT NULL,
        is_root BOOLEAN GENERATED ALWAYS AS (parent_id IS NULL) VIRTUAL,
        FOREIGN KEY (parent_id) REFERENCES accounts(id)
        CHECK(parent_id != id)
        CHECK(account_type_code >= 0 AND account_type_code <= 5)
        CHECK(is_operable IN (0, 1))
    );)";

const auto createEntriesTable =
    R"(CREATE TABLE IF NOT EXISTS journal_entries ( 
        id INTEGER PRIMARY KEY NOT NULL,
        entry_date TEXT NOT NULL,
        detail TEXT,
        CHECK(date(entry_date) IS NOT NULL)
    );)";

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
    );)";

const auto tablesCreated =
    R"(SELECT COUNT(*) FROM sqlite_master 
    WHERE type = 'table' 
    AND name IN 
    ('account_types', 'accounts', 'journal_entries', 'journal_entry_movements');)";

} // namespace schema

namespace inserts
{
const auto insertAccount =
    R"(INSERT INTO accounts(account_type_code, parent_id, name, is_operable)
    VALUES(?, ?, ?, ?);)";
}

} // namespace queries