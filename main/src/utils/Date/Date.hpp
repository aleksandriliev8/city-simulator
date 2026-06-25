#pragma once

#include <string>

class Date {
private:
    int day;
    int month;
    int year;

    static bool isLeapYear(int year);
    static int daysInMonth(int month, int year);
    static void validate(int day, int month, int year);

    static long long toOrdinalDay(int day, int month, int year);
    static void fromOrdinalDay(long long ord, int& day, int& month, int& year);

public:
    Date();
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    Date addDays(int n) const;
    bool isFirstOfMonth() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;

    std::string toString() const;
};