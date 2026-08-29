#pragma once

#include <string>

// Immutable date class with calendar arithmetic via the Julian Day algorithm.
// Default constructor initializes to today's system date.
class Date {
private:
    int day;
    int month;
    int year;

    static bool isLeapYear(int year);
    static int daysInMonth(int month, int year);
    static void validate(int day, int month, int year);

    // Julian Day conversions for efficient date arithmetic (addDays, comparisons)
    static long long toOrdinalDay(int day, int month, int year);
    static void fromOrdinalDay(long long ord, int& day, int& month, int& year);

public:
    Date();
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    Date addDays(int days) const;
    bool isFirstOfMonth() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    std::string toString() const;
};