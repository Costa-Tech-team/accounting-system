#include "account_node.hpp"

#include <stdexcept>

AccountNode::AccountNode(AccountNode &&other) noexcept
  : parent{other.parent},
    children{std::move(other.children)},
    displayName{std::move(other.displayName)},
    category{other.category},
    currentability{other.currentability}
{
    updateChildren();
}

AccountNode &AccountNode::operator=(AccountNode &&other) noexcept
{
    parent = other.parent;
    children = std::move(other.children);
    displayName = std::move(other.displayName);
    category = other.category;
    currentability = other.currentability;
    updateChildren();
    return *this;
}

AccountNode::AccountNode(std::string_view displayName, uint8_t code,
                         Category category, Currentability currentability,
                         bool postable)
  : parent{nullptr},
    displayName{displayName},
    category{category},
    currentability{currentability},
    nodeSubcode{code},
    postable{postable}
{
    validate();
}

AccountNode::AccountNode(std::string_view displayName, AccountNode &parent,
                         bool postable)
  : parent{&parent},
    displayName{displayName},
    nodeSubcode{static_cast<uint8_t>(parent.getChildren().size() + 1)},
    postable{postable}
{}

std::string_view AccountNode::getDisplayName() const { return displayName; }

std::string AccountNode::getCode() const
{
    std::string code;
    if (parent == nullptr)
    {
        code = std::to_string(subCode(getCategory())) + ".";
        if (currentability != Currentability::none)
        {
            uint8_t currentabilityNumber = subCode(getCurrentability());
            code += std::to_string(currentabilityNumber) + "." +
                    std::to_string(nodeSubcode);
        }
    } else
    {
        code = parent->getCode() + "." + std::to_string(nodeSubcode);
    }
    return code;
}

uint8_t AccountNode::getNodeSubcode() const { return nodeSubcode; }

AccountNode::Nature AccountNode::getNature(AccountNode::Category category)
{
    AccountNode::Nature result;
    switch (category)
    {
    case AccountNode::Category::asset:
    case AccountNode::Category::expenseOrCost:
        result = AccountNode::Nature::debtor;
        break;
    case AccountNode::Category::liability:
    case AccountNode::Category::netWorth:
    case AccountNode::Category::revenue:
        result = AccountNode::Nature::creditor;
        break;
    }
    return result;
}

AccountNode::Nature AccountNode::getNature() const
{
    return parent ? AccountNode::getNature(parent->getCategory()) :
                    AccountNode::getNature(*category);
}

AccountNode::Category AccountNode::getCategory() const
{
    return parent ? parent->getCategory() : *category;
}

AccountNode::Currentability AccountNode::getCurrentability() const
{
    return parent ? parent->getCurrentability() : *currentability;
}

AccountNode *AccountNode::getParent() { return parent; }

const AccountNode *AccountNode::getParent() const { return parent; }

std::span<AccountNode> AccountNode::getChildren() { return children; }

std::span<const AccountNode> AccountNode::getChildren() const
{
    return children;
}

void AccountNode::setDisplayName(std::string_view name) { displayName = name; }

void AccountNode::setPostable(bool postable) { this->postable = postable; }

void AccountNode::addChild(AccountNode &&account)
{
    children.push_back(std::move(account));
    children.back().parent = this;
    for (auto &child : children)
    {
        child.updateChildren();
    }
}

void AccountNode::validate()
{
    bool assetOrLiability =
        category == Category::asset || category == Category::liability;
    bool hasCurrentability = currentability != Currentability::none;
    if (assetOrLiability != hasCurrentability)
    {
        throw std::invalid_argument(
            "Account currentability is invalid for its category");
    }
}

void AccountNode::updateChildren()
{
    for (auto &child : children)
    {
        child.parent = this;
    }
}

uint8_t subCode(AccountNode::Category category)
{
    return static_cast<uint8_t>(category);
}

uint8_t subCode(AccountNode::Currentability currentability)
{
    return static_cast<uint8_t>(currentability);
}
