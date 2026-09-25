#include "account_node.hpp"

#include <cassert>
#include <cstdint>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

AccountNode::AccountNode(AccountNode &&other) noexcept
  : parent{other.parent},
    rootData{other.rootData},
    children{std::move(other.children)},
    displayName{std::move(other.displayName)},
    postable{other.postable},
    description(other.description)
{
    updateChildren();
}

AccountNode &AccountNode::operator=(AccountNode &&other) noexcept
{
    parent = other.parent;
    rootData = other.rootData;
    children = std::move(other.children);
    displayName = std::move(other.displayName);
    description = std::move(other.description);
    postable = other.postable;
    updateChildren();
    return *this;
}

AccountNode::AccountNode(std::string_view displayName, uint8_t code,
                         Category category, Currentability currentability,
                         bool postable, std::string_view description)
  : parent{nullptr},
    rootData{RootNodeData(category, currentability, code)},
    displayName{displayName},
    postable{postable},
    description{description}
{
    validate();
}

std::string_view AccountNode::getDisplayName() const { return displayName; }

std::string AccountNode::getCode() const
{
    std::string code;
    if (parent == nullptr)
    {
        code = std::to_string(categoryCode(getCategory())) + ".";
        if (getCurrentability() != Currentability::none)
        {
            uint8_t currentabilityNumber =
                currentabilityCode(getCurrentability());
            code += std::to_string(currentabilityNumber) + "." +
                    std::to_string(getNodeSubcode());
        }
    } else
    {
        code = parent->getCode() + "." + std::to_string(getNodeSubcode());
    }
    return code;
}

uint8_t AccountNode::getNodeSubcode() const
{
    if (parent == nullptr)
    {
        return rootData->nodeSubcode;
    }
    auto firstSibling = std::to_address(parent->getChildren().cbegin());

    return static_cast<uint8_t>(std::distance(firstSibling, this)) + 1;
}

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
                    AccountNode::getNature(rootData->category);
}

AccountNode::Category AccountNode::getCategory() const
{
    return parent ? parent->getCategory() : rootData->category;
}

AccountNode::Currentability AccountNode::getCurrentability() const
{
    return parent ? parent->getCurrentability() : rootData->currentability;
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

AccountNode &AccountNode::addChild(std::string_view displayName, bool postable,
                                   std::string_view descripion)
{
    children.emplace_back(AccountNode(this, displayName, postable, descripion));
    for (auto &child : children)
    {
        child.updateChildren();
    }
    return children.back();
}

AccountNode::AccountNode(AccountNode *parent, std::string_view displayName,
                         bool postable, std::string_view description)
  : parent{parent},
    rootData{std::nullopt},
    displayName{displayName},
    postable{postable},
    description{description}
{}

void AccountNode::validate() const
{
    bool assetOrLiability = getCategory() == Category::asset ||
                            getCategory() == Category::liability;
    bool hasCurrentability = getCurrentability() != Currentability::none;
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

uint8_t categoryCode(AccountNode::Category category)
{
    return static_cast<uint8_t>(category);
}

uint8_t currentabilityCode(AccountNode::Currentability currentability)
{
    return static_cast<uint8_t>(currentability);
}
