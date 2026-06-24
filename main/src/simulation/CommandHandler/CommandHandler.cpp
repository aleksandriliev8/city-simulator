#include "CommandHandler.hpp"
#include "../Paginator/Paginator.hpp"
#include <iostream>
#include <sstream>
#include <string>

CommandHandler::CommandHandler(Simulation& simulation) : simulation(simulation) {
}

bool CommandHandler::parseAndExecute(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "generate") {
        int rows, cols;
        if (!(iss >> rows >> cols)) {
            std::cout << "Usage: generate <n> <m>" << std::endl;
            return true;
        }
        handleGenerate(rows, cols);
    }
    else if (command == "add") {
        int row, col, happiness, money, life;
        std::string name, job;
        if (!(iss >> row >> col >> name >> job >> happiness >> money >> life)) {
            std::cout << "Usage: add <n> <m> <name> <job> <happiness> <money> <life>" << std::endl;
            return true;
        }
        handleAdd(row, col, name, job, happiness, money, life);
    }
    else if (command == "remove") {
        int row, col;
        std::string name;
        if (!(iss >> row >> col >> name)) {
            std::cout << "Usage: remove <n> <m> <name>" << std::endl;
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
            std::cout << "Usage: stat <option>" << std::endl;
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
            std::cout << "Usage: save <name>" << std::endl;
            return true;
        }
        handleSave(filename);
    }
    else if (command == "load") {
        std::string filename;
        if (!(iss >> filename)) {
            std::cout << "Usage: load <name>" << std::endl;
            return true;
        }
        handleLoad(filename);
    }
    else if (command == "exit") {
        return false;
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
    }

    return true;
}

void CommandHandler::run() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (!parseAndExecute(line)) {
            if (simulation.isUnsaved()) {
                std::cout << "You have unsaved changes. Save before exit? (y/n): ";
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
        std::cout << "Error: dimensions must be positive" << std::endl;
        return;
    }
    simulation.generate(rows, cols);
    std::cout << simulation.getCity()->getCurrentDate().toString() << std::endl;
}

void CommandHandler::handleAdd(int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }
    if (happiness < 0 || happiness > 100) {
        std::cout << "Error: happiness must be between 0 and 100" << std::endl;
        return;
    }
    if (money < 0) {
        std::cout << "Error: money must be non-negative" << std::endl;
        return;
    }
    if (life < 0 || life > 100) {
        std::cout << "Error: life must be between 0 and 100" << std::endl;
        return;
    }
    if (simulation.addResident(row, col, name, job, happiness, money, life)) {
        std::cout << "Resident added successfully" << std::endl;
    }
    else {
        std::cout << "Error: could not add resident" << std::endl;
    }
}

void CommandHandler::handleRemove(int row, int col, const std::string& name) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }
    if (simulation.removeResident(row, col, name)) {
        std::cout << "Resident removed successfully" << std::endl;
    }
    else {
        std::cout << "Error: resident not found" << std::endl;
    }
}

void CommandHandler::handleStep(int days) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }
    try {
        simulation.step(days);
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void CommandHandler::handleInfo() {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }

    City* city = simulation.getCity();
    Paginator paginator;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            paginator.addLine("Location " + std::to_string(i) + " " + std::to_string(j) + ":");
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) {
                paginator.addLine("    Empty");
                continue;
            }
            paginator.addLine("    " + building->getType() + ":");
            for (int k = 0; k < building->getResidentCount(); k++) {
                Resident* resident = building->getResidents()[k];
                paginator.addLine("        " + resident->getName());
                paginator.addLine("            Profession: " + resident->getProfession()->getName());
                paginator.addLine("            Happiness:  " + std::to_string(resident->getHappiness()));
                paginator.addLine("            Money:      " + std::to_string(resident->getMoney()));
                paginator.addLine("            Life:       " + std::to_string(resident->getLife()));
            }
        }
    }

    paginator.display();
}

void CommandHandler::handleInfo(int row, int col) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }

    City* city = simulation.getCity();

    if (!city->isValidPosition(row, col)) {
        std::cout << "Error: invalid position" << std::endl;
        return;
    }

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) {
        std::cout << "Location (" << row << ", " << col << ") is empty" << std::endl;
        return;
    }

    Paginator paginator;
    paginator.addLine("Type:        " + building->getType());
    paginator.addLine("Rent:        " + std::to_string((int)building->getRent(city->getRows(), city->getCols())) + " EUR");
    paginator.addLine("Capacity:    " + std::to_string(building->getCapacity()));
    paginator.addLine("Free slots:  " + std::to_string(building->getFreeSlots()));
    paginator.addLine("Residents:");
    for (int k = 0; k < building->getResidentCount(); k++) {
        paginator.addLine("    " + building->getResidents()[k]->getName());
    }
    paginator.display();
}

void CommandHandler::handleInfo(int row, int col, const std::string& name) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }

    City* city = simulation.getCity();

    if (!city->isValidPosition(row, col)) {
        std::cout << "Error: invalid position" << std::endl;
        return;
    }

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) {
        std::cout << "Error: no building at (" << row << ", " << col << ")" << std::endl;
        return;
    }

    Resident* resident = building->getResident(name);
    if (resident == nullptr) {
        std::cout << "Error: resident " << name << " not found" << std::endl;
        return;
    }

    Paginator paginator;
    paginator.addLine("Name:       " + resident->getName());
    paginator.addLine("Profession: " + resident->getProfession()->getName());
    paginator.addLine("Happiness:  " + std::to_string(resident->getHappiness()));
    paginator.addLine("Money:      " + std::to_string(resident->getMoney()));
    paginator.addLine("Life:       " + std::to_string(resident->getLife()));
    paginator.addLine("History:");
    for (int h = 0; h < (int)resident->getHistory().size(); h++) {
        paginator.addLine("    " + resident->getHistory()[h].toString());
    }
    paginator.display();
}

void CommandHandler::handleStat(const std::string& option) {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
        return;
    }

    City* city = simulation.getCity();

    if (option != "happiness" && option != "money" && option != "life" && option != "profession") {
        std::cout << "Error: invalid option. Use happiness, money, life or profession" << std::endl;
        return;
    }

    int total = 0;
    int min = 101;
    int max = -1;
    int count = 0;

    int teachers = 0, programmers = 0, miners = 0, unemployed = 0, students = 0;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;

            for (int k = 0; k < building->getResidentCount(); k++) {
                Resident* resident = building->getResidents()[k];
                count++;

                int value = 0;
                if (option == "happiness") value = resident->getHappiness();
                else if (option == "money")     value = resident->getMoney();
                else if (option == "life")      value = resident->getLife();
                else if (option == "profession") {
                    std::string prof = resident->getProfession()->getName();
                    if (prof == "Teacher")    teachers++;
                    else if (prof == "Programmer") programmers++;
                    else if (prof == "Miner")      miners++;
                    else if (prof == "Unemployed") unemployed++;
                    else if (prof == "Student")    students++;
                    continue;
                }

                total += value;
                if (value < min) min = value;
                if (value > max) max = value;
            }
        }
    }

    Paginator paginator;

    if (option == "profession") {
        paginator.addLine("Total residents: " + std::to_string(count));
        paginator.addLine("Teachers:        " + std::to_string(teachers));
        paginator.addLine("Programmers:     " + std::to_string(programmers));
        paginator.addLine("Miners:          " + std::to_string(miners));
        paginator.addLine("Unemployed:      " + std::to_string(unemployed));
        paginator.addLine("Students:        " + std::to_string(students));
    }
    else {
        if (count == 0) {
            std::cout << "No residents in simulation" << std::endl;
            return;
        }
        paginator.addLine("Total residents: " + std::to_string(count));
        paginator.addLine("Average:         " + std::to_string(total / count));
        paginator.addLine("Min:             " + std::to_string(min));
        paginator.addLine("Max:             " + std::to_string(max));
    }

    paginator.display();
}

void CommandHandler::handleStatBuildings() {
    if (!simulation.hasCity()) {
        std::cout << "Error: no active simulation" << std::endl;
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
            if (type == "Modern")    modern++;
            else if (type == "Panel")     panel++;
            else if (type == "Dormitory") dormitory++;

            double distance = std::sqrt((i - centerRow) * (i - centerRow) + (j - centerCol) * (j - centerCol));
            if (distance <= minDim / 8.0)          central++;
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
        std::cout << "Simulation saved successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void CommandHandler::handleLoad(const std::string& filename) {
    try {
        simulation.load(filename);
        std::cout << "Simulation loaded successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}