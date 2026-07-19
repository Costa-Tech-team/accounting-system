#include "account.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <sys/types.h>

Account::Account(Category category, Currentability currentability,
                 uint8_t accountCode, uint8_t subcategoryCode,
                 uint8_t subaccountCode)
  : accountCategory{category},
    currentability{currentability},
    subcategoryCode{subcategoryCode},
    accountCode{accountCode},
    subaccountCode{subaccountCode}
{}

std::string Account::getCode() const
{
    uint8_t categoryNumber = static_cast<uint8_t>(accountCategory) + 1;
    std::string code{std::to_string(categoryNumber) + "."};
    if (currentability != Currentability::none)
    {
        uint8_t currentabilityNumber = static_cast<uint8_t>(currentability);
        code += std::to_string(currentabilityNumber) + ".";
    }
    if (subcategoryCode != 0)
    {
        code += std::to_string(subcategoryCode) + ".";
    }
    code += std::to_string(accountCode) + ".";
    if (subaccountCode != 0)
    {
        code += std::to_string(subaccountCode);
    }
    return code;
}

Account::Category Account::getCategory() const { return accountCategory; }

Account::Currentability Account::getCurrentability() const
{
    return currentability;
}

uint8_t Account::getSubcategory() const { return subcategoryCode; }

uint8_t Account::getAccountCode() const { return accountCode; }

uint8_t Account::getSubaccountCode() const { return subaccountCode; }