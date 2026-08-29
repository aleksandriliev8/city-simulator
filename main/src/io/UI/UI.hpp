#pragma once

#include <string>
#include "../../../src/core/Building/Building.hpp"
#include "../../../src/core/Resident/Resident.hpp"

namespace UI {
    void printHeader();
    void printHelp();
    void printSuccess(const std::string& message);
    void printError(const std::string& message);
    void printWarning(const std::string& message);
    void printPrompt();

    void printCityCreated(const std::string& name, const std::string& date);
    void printCurrentDate(const std::string& date);

    std::string formatLocation(int row, int col, const std::string& buildingType);
    std::string formatBuildingInfo(const std::string& type, int rent, int capacity, int freeSlots);
    std::string formatResident(const Resident& resident);
    std::string formatResidentFull(const Resident& resident);
    std::string formatHistoryEntry(const std::string& date, const std::string& description);

    void printStepResult(int zeroHappiness, int zeroLife, int zeroMoney);
    void printPageBreak(int current, int total);
}