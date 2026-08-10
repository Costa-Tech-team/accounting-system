#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#pragma once

/// @brief represents a node in an account chart. This means an instance can be
/// a postable account or a subcategory of accounts. Currentability and
/// categories are stored as enum fields.
///
/// For example, the user can group several banks in a Bank account and be able
/// to only use the accounts for specific banks on their chart, or can
/// categorize assets dependeing on wether they're money in cash or a bank
/// account, debt owed to them, etc.
class AccountNode
{
  public:
    /// This enum has the categories of accountable accounts.
    enum class Category
    {
        asset = 1,
        liability,
        revenue,
        expenseOrCost,
        netWorth,
    };

    /// This enum contain the possible values of currentability in accountable
    /// class. if the category is not an asset or liability, "none" is the
    /// appropiate value.
    enum class Currentability
    {
        none,
        current,
        fixed
    };

    /// This enum contains the two possible natures of accounts based on it's
    /// category: debtor for assets, expenses and costs; creditor for liability,
    /// revenue and net worth.
    enum class Nature
    {
        debtor,
        creditor
    };

    /// constructor for an account without parent.
    /// @param displayName: the name of the account to display to the user
    /// @param code: the code of the node provided, must be unique.
    /// @param category: the category of the account.
    /// @param whether: the account is current or fixed, only if category is
    /// asset or liability; otherwise, is must be none.
    /// @param postable: whether the account can be used directly to note
    /// movements in accounting entries or not.
    AccountNode(std::string_view displayName, uint8_t code, Category category,
                Currentability currentability = Currentability::none,
                bool postable = false);

    /// constructor for an account with a parent. The node code is determined by
    /// the parent.
    /// @param displayName: the name of the account to display to the user
    /// @param parent: the parent node of the account, the child will inherit
    /// category and currentability from it.
    /// @param postable: whether the account can be used directly to note
    /// movements in accounting entries or not.
    AccountNode(std::string_view displayName, AccountNode &parent,
                bool postable = true);

    /// @return the name of the account
    std::string_view getDisplayName() const;

    /// @return a code formated as "X.X..." where every number notes category,
    /// currentability (only if asset or liability), and then parents in
    /// hierarchical order and at last the subcode of the node itself.
    std::string getCode() const;

    /// @return a numerical code of the node itself with any of the codes on any
    /// parents.
    uint8_t getNodeSubcode() const;

    /// @return the nature of the account given its category.
    Nature getNature();

    /// \overload
    static Nature getNature(Category category);

    /// @return the category of the account, if the account has a parent the
    /// category will match to that of the parent.
    Category getCategory() const;

    /// @return the currentability of the account, if the account has a parent
    /// the currentability will match to that of the parent.
    Currentability getCurrentability() const;

    /// @return the parent account, null if the account has no parent.
    const AccountNode *getParent() const;

    /// @return a view to all the children accounts, if the Account doesn't have
    /// children, returns a empty object.
    std::span<const AccountNode> getChildren() const;

    /// Adds a child account that is owned by the parent.
    void addChild(AccountNode &&account);

  private:
    void validate();

    const AccountNode *parent;
    std::vector<AccountNode> children;

    std::string displayName;

    std::optional<Category> category;
    std::optional<Currentability> currentability;

    uint8_t nodeSubcode;
    bool postable;
};

uint8_t subCode(AccountNode::Category category);

uint8_t subCode(AccountNode::Currentability currentability);