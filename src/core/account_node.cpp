#include "account_node.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/types.h>

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

AccountNode::Nature AccountNode::getNature()
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

const AccountNode *AccountNode::getParent() const { return parent; }

std::span<const AccountNode> AccountNode::getChildren() const
{
    return children;
}

void AccountNode::addChild(AccountNode &&account)
{
    children.push_back(std::move(account));
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

uint8_t subCode(AccountNode::Category category)
{
    return static_cast<uint8_t>(category);
}

uint8_t subCode(AccountNode::Currentability currentability)
{
    return static_cast<uint8_t>(currentability);
}