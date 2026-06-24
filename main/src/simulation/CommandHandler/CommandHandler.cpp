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
    // TODO: Paginator
}

void CommandHandler::handleInfo(int row, int col) {
    // TODO: Paginator
}

void CommandHandler::handleInfo(int row, int col, const std::string& name) {
    // TODO: Paginator
}

void CommandHandler::handleStat(const std::string& option) {
    // TODO: statistics
}

void CommandHandler::handleStatBuildings() {
    // TODO: statistics
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