#include "UI.hpp"
#include <iostream>

void UI::printHeader() {
    std::cout << "=================================" << std::endl;
    std::cout << "    City Simulation v1.0" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << std::endl;
}

void UI::printHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  generate <n> <m>                               - Generate a new city" << std::endl;
    std::cout << "  add <n> <m> <name> <job> <hp> <money> <life>  - Add a resident" << std::endl;
    std::cout << "  remove <n> <m> <name>                         - Remove a resident" << std::endl;
    std::cout << "  step [n]                                       - Advance n days (negative = go back)" << std::endl;
    std::cout << "  info                                           - Full simulation info" << std::endl;
    std::cout << "  info <x> <y>                                   - Building info" << std::endl;
    std::cout << "  info <x> <y> <name>                           - Resident info" << std::endl;
    std::cout << "  stat <happiness|money|life|profession>         - Statistics" << std::endl;
    std::cout << "  stat buildings                                 - Building statistics" << std::endl;
    std::cout << "  save <name>                                    - Save simulation" << std::endl;
    std::cout << "  load <name>                                    - Load simulation" << std::endl;
    std::cout << "  exit                                           - Exit" << std::endl;
}

void UI::printSuccess(const std::string& message) {
    std::cout << "OK: " << message << std::endl;
}

void UI::printError(const std::string& message) {
    std::cout << "Error: " << message << std::endl;
}

void UI::printWarning(const std::string& message) {
    std::cout << "Warning: " << message << std::endl;
}

void UI::printPrompt() {
    std::cout << "> ";
}

void UI::printCityCreated(const std::string& name, const std::string& date) {
    std::cout << "City '" << name << "' created." << std::endl;
    std::cout << "Start date: " << date << std::endl;
}

void UI::printCurrentDate(const std::string& date) {
    std::cout << "Current date: " << date << std::endl;
}

std::string UI::formatLocation(int row, int col, const std::string& buildingType) {
    return "Location (" + std::to_string(row) + ", " + std::to_string(col) + ") - " + buildingType;
}

std::string UI::formatBuildingInfo(const std::string& type, int rent, int capacity, int freeSlots) {
    std::string result;
    result += "Type:     " + type + "\n";
    result += "Rent:     " + std::to_string(rent) + " EUR\n";
    result += "Capacity: " + std::to_string(capacity) + " | Free: " + std::to_string(freeSlots);
    return result;
}

std::string UI::formatResident(const Resident& resident) {
    return "  - " + resident.getName() +
        " (" + resident.getProfession()->getName() + ")" +
        " | Happiness: " + std::to_string(resident.getHappiness()) +
        " | Money: " + std::to_string(resident.getMoney()) +
        " | Life: " + std::to_string(resident.getLife());
}

std::string UI::formatResidentFull(const Resident& resident) {
    std::string result;
    result += "Name:       " + resident.getName() + "\n";
    result += "Profession: " + resident.getProfession()->getName() + "\n";
    result += "Happiness:  " + std::to_string(resident.getHappiness()) + "\n";
    result += "Money:      " + std::to_string(resident.getMoney()) + "\n";
    result += "Life:       " + std::to_string(resident.getLife()) + "\n";
    result += "History:\n";
    for (int i = 0; i < (int)resident.getHistory().size(); i++) {
        result += "  " + resident.getHistory()[i].toString() + "\n";
    }
    return result;
}

std::string UI::formatHistoryEntry(const std::string& date, const std::string& description) {
    return "  [" + date + "] " + description;
}

void UI::printStepResult(int zeroHappiness, int zeroLife, int zeroMoney) {
    std::cout << "Residents with zero happiness: " << zeroHappiness << std::endl;
    std::cout << "Residents with zero life:      " << zeroLife << std::endl;
    std::cout << "Residents with zero money:     " << zeroMoney << std::endl;
}

void UI::printPageBreak(int current, int total) {
    std::cout << "--- Page " << current << "/" << total << " --- [Enter] next | [q] quit ---" << std::endl;
}