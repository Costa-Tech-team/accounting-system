#include "account.hpp"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/types.h>

Account::Account(std::string_view displayName, Category category,
                 Currentability currentability, uint8_t accountCode,
                 const Superaccount *superaccount)
  : displayName{displayName},
    category{category},
    currentability{currentability},
    subcategory{nullptr},
    superaccount{superaccount},
    accountCode{accountCode}
{
    validate();
}

Account::Account(std::string_view displayName, const Subcategory &subcategory,
                 uint8_t accountCode, const Superaccount *superaccount)
  : displayName{displayName},
    category{subcategory.category},
    currentability{subcategory.currentability},
    subcategory{&subcategory},
    superaccount{superaccount},
    accountCode{accountCode}
{
    validate();
}

Account::Nature Account::getNature(Account::Category category)
{
    Account::Nature result;
    switch (category)
    {
    case Account::Category::asset:
    case Account::Category::expenseOrCost:
        result = Account::Nature::debtor;
        break;
    case Account::Category::liability:
    case Account::Category::netWorth:
    case Account::Category::revenue:
        result = Account::Nature::creditor;
        break;
    }
    return result;
}

Account::Nature Account::getNature() { return Account::getNature(category); }

std::string_view Account::getDisplayName() const { return displayName; }

std::string Account::getCode() const
{
    uint8_t categoryNumber = subCode(category);
    std::string code{std::to_string(categoryNumber) + "."};
    if (currentability != Currentability::none)
    {
        uint8_t currentabilityNumber = subCode(currentability);
        code += std::to_string(currentabilityNumber) + ".";
    }
    if (subcategory != nullptr)
    {
        code += std::to_string(subcategory->code) + ".";
    }
    if (superaccount != nullptr)
    {
        code += std::to_string(superaccount->code) + ".";
    }
    code += std::to_string(accountCode);
    return code;
}

Account::Category Account::getCategory() const { return category; }

Account::Currentability Account::getCurrentability() const
{
    return currentability;
}

const Superaccount *Account::getSuperaccount() const { return superaccount; }

const Subcategory *Account::getSubcategory() const { return subcategory; }

uint8_t Account::getAccountCode() const { return accountCode; }

void Account::validate()
{
    bool assetOrLiability =
        category == Category::asset || category == Category::liability;
    bool hasCurrentability = currentability != Currentability::none;
    if (assetOrLiability != hasCurrentability)
    {
        throw std::invalid_argument(
            "Account currentability is invalid por its category");
    }
    if (accountCode == 0)
    {
        throw std::invalid_argument("Account code must be above 0");
    }
}

uint8_t subCode(Account::Category category)
{
    return static_cast<uint8_t>(category);
}

uint8_t subCode(Account::Currentability currentability)
{
    return static_cast<uint8_t>(currentability);
}