#pragma warning (disable:4996)
#include "Date.hpp"
#include <ctime>
#include <stdexcept>

namespace {
    std::string padZero(int value) {
        if (value >= 0 && value < 10) {
            return "0" + std::to_string(value);
        }
        return std::to_string(value);
    }
}

bool Date::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int month, int year) {
    static const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

void Date::validate(int day, int month, int year) {
    if (year <= 0) {
        throw std::invalid_argument("Date: year must be positive");
    }
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Date: month must be between 1 and 12");
    }
    if (day < 1 || day > daysInMonth(month, year)) {
        throw std::invalid_argument("Date: day is out of range for the given month/year");
    }
}

// JuilanDay algorithm invented by Fliegel and Van Flandern

long long Date::toOrdinalDay(int day, int month, int year) {
    long long a = (14 - month) / 12;
    long long y = year + 4800 - a;
    long long m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

// JuilanDay algorithm invented by Fliegel and Van Flandern

void Date::fromOrdinalDay(long long ord, int& day, int& month, int& year) {
    long long a = ord + 32044;
    long long b = (4 * a + 3) / 146097;
    long long c = a - (146097 * b) / 4;
    long long d = (4 * c + 3) / 1461;
    long long e = c - (1461 * d) / 4;
    long long m = (5 * e + 2) / 153;
    day = (int)(e - (153 * m + 2) / 5 + 1);
    month = (int)(m + 3 - 12 * (m / 10));
    year = (int)(100 * b + d - 4800 + m / 10);
}

Date::Date() {
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);
    day = localTime.tm_mday;
    month = localTime.tm_mon + 1;
    year = localTime.tm_year + 1900;
}

Date::Date(int day, int month, int year) {
    validate(day, month, year);
    this->day = day;
    this->month = month;
    this->year = year;
}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

Date Date::addDays(int n) const {
    long long ord = toOrdinalDay(day, month, year) + n;
    int newDay = 0, newMonth = 0, newYear = 0;
    fromOrdinalDay(ord, newDay, newMonth, newYear);
    return Date(newDay, newMonth, newYear);
}

bool Date::isFirstOfMonth() const {
    return day == 1;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year != other.year)   return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

std::string Date::toString() const {
    return padZero(day) + "." + padZero(month) + "." + std::to_string(year);
}