#include "account.hpp"
#include <cassert>
#include <cstdint>
#include <sys/types.h>

static constexpr bool isCategoryBoth(Account::Category category,
                                     Account::Category toCompare1,
                                     Account::Category toCompare2)
{
    return (category & toCompare1) == (category & toCompare2);
}

static bool isAssetOrLiability(Account::Category category)
{
    return (category &
            (Account::Category::asset | Account::Category::liability)) !=
           Account::Category::none;
}

static bool hasQualifications(Account::Category category)
{

    return (category & (Account::Category::current | Account::Category::fixed |
                        Account::Category::debit |
                        Account::Category::credit)) != Account::Category::none;
}

static bool hasOppositesCombination(Account::Category category)
{
    return isCategoryBoth(category, Account::Category::asset,
                          Account::Category::liability) &&
           isCategoryBoth(category, Account::Category::revenue,
                          Account::Category::expense) &&
           isCategoryBoth(category, Account::Category::current,
                          Account::Category::fixed) &&
           isCategoryBoth(category, Account::Category::debit,
                          Account::Category::credit);
}

static bool isValid(Account::Category category)
{
    if (category == Account::Category::none &&
        hasOppositesCombination(category))
    {
        return false;
    }
    bool assetOrLiability = isAssetOrLiability(category);
    bool qualifications = hasQualifications(category);
    return assetOrLiability == qualifications;
}

Account::Account(Category category, std::string_view accountName)
{
    assert(isValid(category));
    accountCategory = category;
    this->accountName = accountName;
}

std::string Account::toString()
{
    std::string result{accountName + " ( )"};
    result[result.length() - 1] =
        accomplishesCategory(Category::debit) ? '+' : '-';
    return result;
}

bool Account::accomplishesCategory(Category category)
{
    return static_cast<uint8_t>(accountCategory & category) != 0;
}

Account::Category operator|(Account::Category lhs, Account::Category rhs)
{
    using T = std::underlying_type_t<Account::Category>;
    auto result = static_cast<Account::Category>(static_cast<T>(lhs) |
                                                 static_cast<T>(rhs));
    assert(isValid(result));
    return result;
}

Account::Category operator&(Account::Category lhs, Account::Category rhs)
{
    using T = std::underlying_type_t<Account::Category>;
    auto result = static_cast<Account::Category>(static_cast<T>(lhs) &
                                                 static_cast<T>(rhs));
    assert(isValid(result));
    return result;
}