#include "CommandHandler.hpp"
#include "../Paginator/Paginator.hpp"
#include "../../utils/Validator/Validator.hpp"
#include <iostream>
#include <sstream>

CommandHandler::CommandHandler(Simulation& simulation) : simulation(simulation) {
}

bool CommandHandler::parseAndExecute(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "generate") {
        int rows, cols;
        if (!(iss >> rows >> cols)) {
            UI::printError("Usage: generate <n> <m>");
            return true;
        }
        handleGenerate(rows, cols);
    }
    else if (command == "add") {
        int row, col, happiness, money, life;
        std::string name, job;
        if (!(iss >> row >> col >> name >> job >> happiness >> money >> life)) {
            UI::printError("Usage: add <n> <m> <name> <job> <happiness> <money> <life>");
            return true;
        }
        handleAdd(row, col, name, job, happiness, money, life);
    }
    else if (command == "remove") {
        int row, col;
        std::string name;
        if (!(iss >> row >> col >> name)) {
            UI::printError("Usage: remove <n> <m> <name>");
            return true;
        }
        handleRemove(row, col, name);
    }
    else if (command == "step") {
        int days = 1;
        iss >> days;
        handleStep(days);
    }
    else if (command == "info") {
        int row, col;
        std::string name;
        if (iss >> row >> col) {
            if (iss >> name) {
                handleInfo(row, col, name);
            }
            else {
                handleInfo(row, col);
            }
        }
        else {
            handleInfo();
        }
    }
    else if (command == "stat") {
        std::string option;
        if (!(iss >> option)) {
            UI::printError("Usage: stat <happiness|money|life|profession|buildings>");
            return true;
        }
        if (option == "buildings") {
            handleStatBuildings();
        }
        else {
            handleStat(option);
        }
    }
    else if (command == "save") {
        std::string filename;
        if (!(iss >> filename)) {
            UI::printError("Usage: save <name>");
            return true;
        }
        handleSave(filename);
    }
    else if (command == "load") {
        std::string filename;
        if (!(iss >> filename)) {
            UI::printError("Usage: load <name>");
            return true;
        }
        handleLoad(filename);
    }
    else if (command == "help") {
        UI::printHelp();
    }
    else if (command == "exit") {
        return false;
    }
    else {
        UI::printError("Unknown command '" + command + "'. Type 'help' for available commands.");
    }

    return true;
}

void CommandHandler::run() {
    UI::printHeader();
    UI::printHelp();
    std::cout << std::endl;

    std::string line;
    while (true) {
        UI::printPrompt();
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        if (!parseAndExecute(line)) {
            if (simulation.isUnsaved()) {
                UI::printWarning("You have unsaved changes. Save before exit? (y/n): ");
                std::string answer;
                std::getline(std::cin, answer);
                if (answer == "y" || answer == "Y") {
                    std::cout << "Enter filename: ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    handleSave(filename);
                }
            }
            break;
        }
    }
}

void CommandHandler::handleGenerate(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        UI::printError("Dimensions must be positive.");
        return;
    }
    simulation.generate(rows, cols);
    UI::printCityCreated(simulation.getCity()->getName(), simulation.getCity()->getCurrentDate().toString());
}

void CommandHandler::handleAdd(int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life) {
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

void CommandHandler::handleRemove(int row, int col, const std::string& name) {
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

void CommandHandler::handleStep(int days) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
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

void CommandHandler::handleInfo() {
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

void CommandHandler::handleInfo(int row, int col) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    if (!city->isValidPosition(row, col)) {
        UI::printError("Invalid position (" + std::to_string(row) + ", " + std::to_string(col) + ").");
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

void CommandHandler::handleInfo(int row, int col, const std::string& name) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    if (!city->isValidPosition(row, col)) {
        UI::printError("Invalid position (" + std::to_string(row) + ", " + std::to_string(col) + ").");
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
    paginator.addLine(UI::formatResidentFull(*resident));
    paginator.display();
}

void CommandHandler::handleStat(const std::string& option) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    if (option != "happiness" && option != "money" && option != "life" && option != "profession") {
        UI::printError("Invalid option. Use: happiness, money, life, profession.");
        return;
    }

    City* city = simulation.getCity();
    int total = 0, min = 101, max = -1, count = 0;
    int teachers = 0, programmers = 0, miners = 0, unemployed = 0, students = 0;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;
            for (int k = 0; k < building->getResidentCount(); k++) {
                Resident* resident = building->getResidents()[k];
                count++;
                if (option == "profession") {
                    std::string prof = resident->getProfession()->getName();
                    if (prof == "Teacher")         teachers++;
                    else if (prof == "Programmer") programmers++;
                    else if (prof == "Miner")      miners++;
                    else if (prof == "Unemployed") unemployed++;
                    else if (prof == "Student")    students++;
                    continue;
                }
                int value = 0;
                if (option == "happiness")     value = resident->getHappiness();
                else if (option == "money")    value = resident->getMoney();
                else if (option == "life")     value = resident->getLife();
                total += value;
                if (value < min) min = value;
                if (value > max) max = value;
            }
        }
    }

    Paginator paginator;
    paginator.addLine("Total residents: " + std::to_string(count));
    if (option == "profession") {
        paginator.addLine("Teachers:    " + std::to_string(teachers));
        paginator.addLine("Programmers: " + std::to_string(programmers));
        paginator.addLine("Miners:      " + std::to_string(miners));
        paginator.addLine("Unemployed:  " + std::to_string(unemployed));
        paginator.addLine("Students:    " + std::to_string(students));
    }
    else {
        if (count == 0) { UI::printError("No residents in simulation."); return; }
        paginator.addLine("Average: " + std::to_string(total / count));
        paginator.addLine("Min:     " + std::to_string(min));
        paginator.addLine("Max:     " + std::to_string(max));
    }
    paginator.display();
}

void CommandHandler::handleStatBuildings() {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    int modern = 0, panel = 0, dormitory = 0;
    int central = 0, peripheral = 0, standard = 0;

    double minDim = (double)(city->getRows() < city->getCols() ? city->getRows() : city->getCols());
    double centerRow = city->getRows() / 2.0;
    double centerCol = city->getCols() / 2.0;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;
            std::string type = building->getType();
            if (type == "Modern")          modern++;
            else if (type == "Panel")      panel++;
            else if (type == "Dormitory")  dormitory++;
            double distance = std::sqrt((i - centerRow) * (i - centerRow) + (j - centerCol) * (j - centerCol));
            if (distance <= minDim / 8.0)            central++;
            else if (distance > 6.0 * minDim / 8.0) peripheral++;
            else                                     standard++;
        }
    }

    Paginator paginator;
    paginator.addLine("--- By type ---");
    paginator.addLine("Modern:     " + std::to_string(modern));
    paginator.addLine("Panel:      " + std::to_string(panel));
    paginator.addLine("Dormitory:  " + std::to_string(dormitory));
    paginator.addLine("--- By location ---");
    paginator.addLine("Central:    " + std::to_string(central));
    paginator.addLine("Standard:   " + std::to_string(standard));
    paginator.addLine("Peripheral: " + std::to_string(peripheral));
    paginator.display();
}

void CommandHandler::handleSave(const std::string& filename) {
    try {
        simulation.save(filename);
        UI::printSuccess("Simulation saved to '" + filename + "'.");
    }
    catch (const std::exception& e) {
        UI::printError(e.what());
    }
}

void CommandHandler::handleLoad(const std::string& filename) {
    try {
        simulation.load(filename);
        UI::printSuccess("Simulation '" + filename + "' loaded.");
        UI::printCurrentDate(simulation.getCity()->getCurrentDate().toString());
    }
    catch (const std::exception& e) {
        UI::printError(e.what());
    }
}