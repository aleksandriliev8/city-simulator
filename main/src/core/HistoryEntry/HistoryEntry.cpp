#include "HistoryEntry.hpp"

HistoryEntry::HistoryEntry(const Date& date, const std::string& description)
    : date(date), description(description) {
}

const Date& HistoryEntry::getDate() const {
    return date;
}

const std::string& HistoryEntry::getDescription() const {
    return description;
}

std::string HistoryEntry::toString() const {
    return date.toString() + " - " + description;
}