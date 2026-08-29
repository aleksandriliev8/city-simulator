#include "InfoCommands.hpp"

void InfoCommands::handleInfo(Simulation& simulation) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    Paginator paginator;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) {
                paginator.addLine("Location (" + std::to_string(i) + ", " + std::to_string(j) + ") - Empty");
                continue;
            }
            paginator.addLine(UI::formatLocation(i, j, building->getType()) + ":");
            for (int k = 0; k < building->getResidentCount(); k++) {
                paginator.addLine(UI::formatResident(*building->getResidents()[k]));
            }
        }
    }

    paginator.display();
}

void InfoCommands::handleInfo(Simulation& simulation, int row, int col) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    std::string error;
    if (!Validator::validatePosition(*city, row, col, error)) {
        UI::printError(error);
        return;
    }

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) {
        UI::printError("No building at (" + std::to_string(row) + ", " + std::to_string(col) + ").");
        return;
    }

    Paginator paginator;
    paginator.addLine(UI::formatBuildingInfo(
        building->getType(),
        (int)building->getRent(city->getRows(), city->getCols()),
        building->getCapacity(),
        building->getFreeSlots()
    ));
    paginator.addLine("Residents:");
    for (int k = 0; k < building->getResidentCount(); k++) {
        paginator.addLine(UI::formatResident(*building->getResidents()[k]));
    }
    paginator.display();
}

void InfoCommands::handleInfo(Simulation& simulation, int row, int col, const std::string& name) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    std::string error;
    if (!Validator::validatePosition(*city, row, col, error)) {
        UI::printError(error);
        return;
    }

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) {
        UI::printError("No building at (" + std::to_string(row) + ", " + std::to_string(col) + ").");
        return;
    }

    Resident* resident = building->getResident(name);
    if (resident == nullptr) {
        UI::printError("Resident '" + name + "' not found.");
        return;
    }

    Paginator paginator;
    paginator.addLine("Name:       " + resident->getName());
    paginator.addLine("Profession: " + resident->getProfession()->getName());
    paginator.addLine("Happiness:  " + std::to_string(resident->getHappiness()));
    paginator.addLine("Money:      " + std::to_string(resident->getMoney()));
    paginator.addLine("Life:       " + std::to_string(resident->getLife()));
    paginator.addLine("History:");
    for (int i = 0; i < (int)resident->getHistory().size(); i++) {
        paginator.addLine("  " + resident->getHistory()[i].toString());
    }
    paginator.display();
}