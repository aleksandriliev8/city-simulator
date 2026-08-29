#include "SimCommands.hpp"
#include <iostream>

void SimCommands::handleGenerate(Simulation& simulation, int rows, int cols) {
    std::string error;
    if (!Validator::validateDimensions(rows, cols, error)) {
        UI::printError(error);
        return;
    }
    if (simulation.hasCity() && simulation.isUnsaved()) {
        UI::printWarning("You have unsaved changes. Save before generating? (y/n):");
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "y" || answer == "Y") {
            std::cout << "Enter filename: ";
            std::string filename;
            std::getline(std::cin, filename);
            handleSave(simulation, filename);
        }
    }
    simulation.generate(rows, cols);
    UI::printCityCreated(simulation.getCity()->getName(), simulation.getCity()->getCurrentDate().toString());
}

void SimCommands::handleAdd(Simulation& simulation, int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }
    std::string error;
    if (!Validator::validatePosition(*simulation.getCity(), row, col, error)) { UI::printError(error); return; }
    if (!Validator::validateCharacteristics(happiness, money, life, error)) { UI::printError(error); return; }
    if (!Validator::validateProfession(job, error)) { UI::printError(error); return; }

    if (simulation.addResident(row, col, name, job, happiness, money, life)) {
        UI::printSuccess("Resident '" + name + "' added.");
    }
    else {
        UI::printError("Could not add resident. Building may be full or student in non-dormitory.");
    }
}

void SimCommands::handleRemove(Simulation& simulation, int row, int col, const std::string& name) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }
    if (simulation.removeResident(row, col, name)) {
        UI::printSuccess("Resident '" + name + "' removed.");
    }
    else {
        UI::printError("Resident '" + name + "' not found at (" + std::to_string(row) + ", " + std::to_string(col) + ").");
    }
}

void SimCommands::handleStep(Simulation& simulation, int days) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }
    if (days == 0) {
        UI::printError("Step count must be non-zero.");
        return;
    }
    try {
        int zeroHappiness = 0, zeroLife = 0, zeroMoney = 0;
        simulation.step(days, zeroHappiness, zeroLife, zeroMoney);
        UI::printCurrentDate(simulation.getCity()->getCurrentDate().toString());
        UI::printStepResult(zeroHappiness, zeroLife, zeroMoney);
    }
    catch (const std::exception& e) {
        UI::printError(e.what());
    }
}

void SimCommands::handleSave(Simulation& simulation, const std::string& filename) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }
    try {
        simulation.save(filename);
        UI::printSuccess("Simulation saved to '" + filename + "'.");
    }
    catch (const std::exception& e) {
        UI::printError(e.what());
    }
}

void SimCommands::handleLoad(Simulation& simulation, const std::string& filename) {
    if (simulation.hasCity() && simulation.isUnsaved()) {
        UI::printWarning("You have unsaved changes. Save before loading? (y/n):");
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "y" || answer == "Y") {
            std::cout << "Enter filename: ";
            std::string saveName;
            std::getline(std::cin, saveName);
            handleSave(simulation, saveName);
        }
    }
    try {
        simulation.load(filename);
        UI::printSuccess("Simulation '" + filename + "' loaded.");
        UI::printCurrentDate(simulation.getCity()->getCurrentDate().toString());
    }
    catch (const std::exception& e) {
        UI::printError(e.what());
    }
}