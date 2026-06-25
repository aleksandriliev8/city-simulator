#pragma once

#include <string>
#include "../../utils/Date/Date.hpp"

class HistoryEntry {
private:
    Date date;
    std::string description;

public:
    HistoryEntry(const Date& date, const std::string& description);

    const Date& getDate() const;
    const std::string& getDescription() const;

    std::string toString() const;
};