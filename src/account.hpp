#include <cstdint>
#include <string>
#include <string_view>

/// @brief represents a subcategory of accounts so that the user can group
/// accounts with the same category and currentability.
struct Subcategory
{
    uint8_t code;
    std::string displayName;
};

/// @brief This class contains information about an accountable account.
///
/// it always contains the category, wether its current of fixed and
/// the account code. It can also contain a subcategory of account before
/// the account code (for example if the user wants to disinguish between
/// assets in banks or cash and assets in documents) and a
/// subaccount after the account code for specification of accounts (for
/// example, when the user wants to note differents banks in their
/// operations). Every account has also a
class Account
{
  public:
    /// This enum has the categories of accountable accounts.
    enum class Category : uint8_t
    {
        asset = 1,
        liability,
        revenue,
        expenseOrCost,
        netWorth,
    };

    /// This enum contain the possible values of currentability in accountable
    /// class. if the category is not asset or liability, "none" is the
    /// appropiate value.
    enum class Currentability
    {
        none,
        current,
        fixed
    };

    /// This enum contains the two possible natures of accounts based on it's
    /// category.
    enum class Nature
    {
        debtor,
        creditor
    };

    /// If the category is an asset or a liability, currentability must not
    /// equal none, and if the category is net worth, revenue or expense or
    /// cost, the currentability must equal none. The optional subcategory
    /// and subaccount are defaulted to zero.
    Account(Category category, Currentability currentability,
            uint8_t accountCode, const Subcategory *subcategory = nullptr,
            uint8_t subaccountCode = 0);

    /// @return the nature of any account given its category.
    static Nature getNature(Category category);

    /// \overload
    Nature getNature();

    /// @return a code with the numerical representation of every quality of the
    /// account separed by dots. The context of the code is provided by the
    /// AccountsChart.
    std::string getCode() const;

    Category getCategory() const;
    Currentability getCurrentability() const;
    /// @return nullptr if not subcategory, a Subcategory instance otherwise.
    const Subcategory *getSubcategory() const;
    uint8_t getAccountCode() const;
    /// @return 0 if not subaccount, a code above 0 if there's subaccount
    uint8_t getSubaccountCode() const;

  private:
    bool postable;
    std::string_view displayName;

    Category category;
    Currentability currentability;
    const Subcategory *subcategory;
    uint8_t accountCode;
    uint8_t subaccountCode;
};

uint8_t subCode(Account::Category category);

uint8_t subCode(Account::Currentability currentability);