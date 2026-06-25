#pragma once

#include <string>
#include "../../../src/core/Building/Building.hpp"
#include "../../../src/core/Resident/Resident.hpp"

class UI {
public:
    UI() = delete;

    static void printHeader();
    static void printHelp();
    static void printSuccess(const std::string& message);
    static void printError(const std::string& message);
    static void printWarning(const std::string& message);
    static void printPrompt();

    static void printCityCreated(const std::string& name, const std::string& date);
    static void printCurrentDate(const std::string& date);

    static std::string formatLocation(int row, int col, const std::string& buildingType);
    static std::string formatBuildingInfo(const std::string& type, int rent, int capacity, int freeSlots);
    static std::string formatResident(const Resident& resident);
    static std::string formatResidentFull(const Resident& resident);
    static std::string formatHistoryEntry(const std::string& date, const std::string& description);

    static void printStepResult(int zeroHappiness, int zeroLife, int zeroMoney);
    static void printPageBreak(int current, int total);
};