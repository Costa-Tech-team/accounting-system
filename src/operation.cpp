#include "operation.hpp"

Operation::Operation(double amount, const Account &account, const Date &date)
  : account{account},
    date{date},
    amount{amount}
{}

Account Operation::getAccount() const { return account; }

Operation::Type Operation::getType() const { return type; }

Date Operation::getDate() const { return date; }

double Operation::getAmount() const { return amount; }

Date Operation::currentDate()
{
    return std::chrono::floor<std::chrono::days>(
        std::chrono::system_clock::now());
}