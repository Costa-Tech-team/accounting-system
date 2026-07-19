#include <cstdint>
#include <string>

/// @brief This class contains information about an accountable account.
///
/// it always contains the category, wether its current of fixed and
/// the account code. It can also contain a subcategory of account before
/// the account code (for example if the user wants to disinguish between
/// assets in banks or cash and assets in documents) and a
/// subaccount after the account code for specification of accounts (for
/// example, when the user wants to note differents banks in their
/// operations).
class Account
{
  public:
    /// This enum has the categories of accountable accounts, the last
    /// two items are provided for convenience when checking if a account has
    /// one of two mutually exclusives flags.
    enum class Category : uint8_t
    {
        asset,
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

    /// If the category is an asset or a liability, currentability must not
    /// equal none, and if the category is net worth, revenue or expense or
    /// cost, the currentability must equal none. The optional subcategory and
    /// subaccount are defaulted to zero.
    Account(Category category, Currentability currentability,
            uint8_t accountCode, uint8_t subcategoryCode = 0,
            uint8_t subaccountCode = 0);

    /// @return a code with the numerical representation of every quality of the
    /// account separed by dots. The context of the code is provided by the
    /// AccountsChart.
    std::string getCode() const;

    Category getCategory() const;
    Currentability getCurrentability() const;
    uint8_t getSubcategory() const;
    uint8_t getAccountCode() const;
    uint8_t getSubaccountCode() const;

  private:
    Category accountCategory;
    Currentability currentability;
    uint8_t subcategoryCode;
    uint8_t accountCode;
    uint8_t subaccountCode;
};