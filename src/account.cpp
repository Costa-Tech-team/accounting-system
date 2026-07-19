#include "account.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <sys/types.h>

Account::Account(Category category, Currentability currentability,
                 uint8_t accountCode, const Subcategory *subcategory,
                 uint8_t subaccountCode)
  : category{category},
    currentability{currentability},
    subcategory{subcategory},
    accountCode{accountCode},
    subaccountCode{subaccountCode}
{}

Account::Nature Account::getNature(Account::Category category)
{
    switch (category)
    {
    case Account::Category::asset:
    case Account::Category::expenseOrCost:
        return Account::Nature::debtor;
        break;
    case Account::Category::liability:
    case Account::Category::netWorth:
    case Account::Category::revenue:
        return Account::Nature::creditor;
        break;
    }
}

Account::Nature Account::getNature() { return Account::getNature(category); }

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
    code += std::to_string(accountCode) + ".";
    if (subaccountCode != 0)
    {
        code += std::to_string(subaccountCode);
    }
    return code;
}

Account::Category Account::getCategory() const { return category; }

Account::Currentability Account::getCurrentability() const
{
    return currentability;
}

const Subcategory *Account::getSubcategory() const { return subcategory; }

uint8_t Account::getAccountCode() const { return accountCode; }

uint8_t Account::getSubaccountCode() const { return subaccountCode; }

uint8_t subCode(Account::Category category)
{
    return static_cast<uint8_t>(category);
}

uint8_t subCode(Account::Currentability currentability)
{
    return static_cast<uint8_t>(currentability);
}