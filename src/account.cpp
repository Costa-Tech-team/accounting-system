#include "account.hpp"
#include <cassert>

static constexpr bool isCategoryBoth(Account::Category category,
                                     Account::Category toCompare1,
                                     Account::Category toCompare2)
{
    auto categoryIntersects1 = fullfilsCategory(category, toCompare1);
    auto categoryIntersects2 = fullfilsCategory(category, toCompare2);
    return categoryIntersects1 == categoryIntersects2;
}

static bool hasOppositesCombination(Account::Category category)
{
    return isCategoryBoth(category, Account::Category::asset,
                          Account::Category::liability) ||
           isCategoryBoth(category, Account::Category::revenue,
                          Account::Category::expense);
}

static bool hasExclusivesCombination(Account::Category category)
{
    bool assetOrLiability =
        fullfilsCategory(category, Account::Category::assetOrLiability);
    bool revenueOrExpense =
        fullfilsCategory(category, Account::Category::revenueOrExpense);
    bool netWorth = fullfilsCategory(category, Account::Category::netWorth);
    return 1 < (assetOrLiability + revenueOrExpense + netWorth);
}

bool isValid(Account::Category category)
{
    auto isNotNone = category != Account::Category::none;
    auto oppositesCombination = hasOppositesCombination(category);
    auto exclusivesCombination = hasExclusivesCombination(category);
    return isNotNone && !oppositesCombination && !exclusivesCombination;
}

Account::Account(Category category, std::string_view accountName)
{
    assert(isValid(category));
    accountCategory = category;
    this->accountName = accountName;
}

std::string Account::getAccountName() const { return accountName; }

bool fullfilsCategory(Account::Category category, Account::Category toFullfil)
{
    return (category & toFullfil) != Account::Category::none;
}

Account::Category operator|(Account::Category lhs, Account::Category rhs)
{
    using T = std::underlying_type_t<Account::Category>;
    auto result = static_cast<Account::Category>(static_cast<T>(lhs) |
                                                 static_cast<T>(rhs));
    return result;
}

Account::Category operator&(Account::Category lhs, Account::Category rhs)
{
    using T = std::underlying_type_t<Account::Category>;
    auto result = static_cast<Account::Category>(static_cast<T>(lhs) &
                                                 static_cast<T>(rhs));
    return result;
}