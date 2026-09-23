#include <memory>
#include <vector>

#include <sqlite3.h>

#include "../core/account_node.hpp"

struct sqlite3;
struct sqlite3_stmt;
class AccountsChart;

/// @brief this class operates the accounts table in thedatabase.
class AccountsRepository
{
  public:
    AccountsRepository(sqlite3 *database);
    AccountsRepository(AccountsRepository &&) = default;
    AccountsRepository(const AccountsRepository &) = delete;
    AccountsRepository &operator=(AccountsRepository &&) = default;
    AccountsRepository &operator=(const AccountsRepository &) = delete;
    ~AccountsRepository() = default;

    AccountsChart retrieveChart() const;

  private:
    using AccountIDPair = std::pair<AccountNode, int>;

    void throwSQLerror(int code, const char *sql) const;

    std::vector<AccountIDPair> fillFromTable() const;

    AccountIDPair createRootStep() const;

    void createChildren(AccountNode &account, int id) const;

    int createChildStep(AccountNode &parent) const;

    struct StatementDeleter
    {
        void operator()(sqlite3_stmt *stmt) const noexcept;
    };

    using StmtPointer = std::unique_ptr<sqlite3_stmt, StatementDeleter>;

    StmtPointer insertAccount;
    StmtPointer selectRootAccounts;
    StmtPointer selectAccountChildren;
};
