#include <filesystem>
#include <string>
#include <string_view>

struct sqlite3;
struct sqlite3_stmt;
class SQLite3;

constexpr auto databaseFilename = "accounting_system.db";

/// @brief RAII wrapper around a prepared sqlite3_stmt. It finalizes the
/// statement on destruction. It must be created from a SQLite3 instance.
class Statement
{
  public:
    Statement(sqlite3 *database, std::string_view sql);
    Statement(Statement &&other) noexcept;
    Statement(const Statement &) = delete;
    Statement &operator=(Statement &&other) noexcept;
    Statement &operator=(const Statement &) = delete;
    ~Statement();

    /// Bind parameters (1-indexed). This overload binds a integer.
    void bind(int index, int64_t value);
    /// Bind parameters (1-indexed). This overload binds a double.
    void bind(int index, double value);
    /// Bind parameters (1-indexed). This overload binds a string.
    void bind(int index, std::string_view value);
    /// Bind parameters (1-indexed). This overload binds a null value.
    void bindNull(int index);

    /// Executes the query when called for the first time, then teps to the next
    /// row in the results of the query.
    /// @return true if the step hasn't reached the end of the results.
    bool step();
    /// Resets the statement to its inital state, ready to be re-executed.
    void reset();
    /// Clears the bound results for the statement to be able to bind new ones.
    void clear();

    SQLite3 databaseHandle();
    bool readOnly() const;

    /// @return the column value of the current row indicated by the index. If
    /// the actual type of the column is unmatching with an int64_t, automatic
    /// conversions are performed by the SQLite3 library.
    int64_t columnInt64(int index) const;
    /// @return the column value of the current row indicated by the index. If
    /// the actual type of the column is unmatching with an double, automatic
    /// conversions are performed by the SQLite3 library.
    double columnDouble(int index) const;
    /// @return the column value of the current row indicated by the index. If
    /// the actual type of the column is unmatching with a std::string,
    /// automatic conversions are performed by the SQLite3 library.
    std::string columnText(int index) const;
    /// @return wether the value in the column of the specified index is a null
    /// value.
    bool columnIsNull(int index) const;
    /// @return how much columns does the row have.
    int columnCount() const;

    /// @return the underlying SQLite3 pointer to the statement object. While
    /// getting it is a option, it isn't encouraged; instead, expanding the
    /// wrapper to have the method you need to use is preferred. Releasing the
    /// resource with the C API will lead to an error because this class already
    /// calls the API to release it in its destructor, so this should never be
    /// done.
    sqlite3_stmt *getHandle();

  private:
    sqlite3_stmt *statement;
};

/// @brief RAII wrapper around a sqlite3 database connection. It closes the
/// database connection on destruction.
class SQLite3
{
  public:
    SQLite3(const std::filesystem::path &databasePath = databaseFilename);
    SQLite3(sqlite3 *database);
    SQLite3(SQLite3 &&other) noexcept;
    SQLite3(const SQLite3 &) = delete;
    SQLite3 &operator=(SQLite3 &&other) noexcept;
    SQLite3 &operator=(const SQLite3 &) = delete;
    ~SQLite3();

    // Direct execution without results.
    void exec(std::string_view sql);

    // Prepared statement factory.
    Statement prepare(std::string_view sql);

    /// @return the underlying SQLite3 pointer to the database object. While
    /// getting it is a option, it isn't encouraged for users; instead,
    /// expanding the wrapper to have the method you wanna use is preferred.
    /// Releasing the resource with the C API will lead to an error because this
    /// class already calls the API to release it in its destructor.
    sqlite3 *handle() const { return database; }

  private:
    sqlite3 *database;
};