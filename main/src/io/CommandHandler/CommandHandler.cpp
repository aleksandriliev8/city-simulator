#include "CommandHandler.hpp"
#include "../../utils/StringUtils/StringUtils.hpp"
#include <iostream>

CommandHandler::CommandHandler(Simulation& simulation) : simulation(simulation) {
}

bool CommandHandler::parseAndExecute(const std::string& line) {
    std::vector<std::string> tokens = StringUtils::tokenize(line);
    if (tokens.empty()) return true;

    const std::string& command = tokens[0];

    if (command == "generate") {
        if (tokens.size() != 3) { UI::printError("Usage: generate <n> <m>"); return true; }
        if (!StringUtils::isInt(tokens[1]) || !StringUtils::isInt(tokens[2])) { UI::printError("Dimensions must be integers."); return true; }
        SimCommands::handleGenerate(simulation, StringUtils::toInt(tokens[1]), StringUtils::toInt(tokens[2]));
    }
    else if (command == "add") {
        if (tokens.size() != 8) { UI::printError("Usage: add <n> <m> <name> <job> <happiness> <money> <life>"); return true; }
        if (!StringUtils::isInt(tokens[1]) || !StringUtils::isInt(tokens[2]) || !StringUtils::isInt(tokens[5]) || !StringUtils::isInt(tokens[6]) || !StringUtils::isInt(tokens[7])) {
            UI::printError("Coordinates and characteristics must be integers."); return true;
        }
        SimCommands::handleAdd(simulation, StringUtils::toInt(tokens[1]), StringUtils::toInt(tokens[2]), tokens[3], tokens[4], StringUtils::toInt(tokens[5]), StringUtils::toInt(tokens[6]), StringUtils::toInt(tokens[7]));
    }
    else if (command == "remove") {
        if (tokens.size() != 4) { UI::printError("Usage: remove <n> <m> <name>"); return true; }
        if (!StringUtils::isInt(tokens[1]) || !StringUtils::isInt(tokens[2])) { UI::printError("Coordinates must be integers."); return true; }
        SimCommands::handleRemove(simulation, StringUtils::toInt(tokens[1]), StringUtils::toInt(tokens[2]), tokens[3]);
    }
    else if (command == "step") {
        if (tokens.size() > 1 && !StringUtils::isInt(tokens[1])) { UI::printError("Step count must be an integer."); return true; }
        int days = tokens.size() > 1 ? StringUtils::toInt(tokens[1]) : 1;
        SimCommands::handleStep(simulation, days);
    }
    else if (command == "info") {
        if (tokens.size() == 1)      InfoCommands::handleInfo(simulation);
        else if (tokens.size() == 3) {
            if (!StringUtils::isInt(tokens[1]) || !StringUtils::isInt(tokens[2])) { UI::printError("Coordinates must be integers."); return true; }
            InfoCommands::handleInfo(simulation, StringUtils::toInt(tokens[1]), StringUtils::toInt(tokens[2]));
        }
        else if (tokens.size() == 4) {
            if (!StringUtils::isInt(tokens[1]) || !StringUtils::isInt(tokens[2])) { UI::printError("Coordinates must be integers."); return true; }
            InfoCommands::handleInfo(simulation, StringUtils::toInt(tokens[1]), StringUtils::toInt(tokens[2]), tokens[3]);
        }
        else UI::printError("Usage: info | info <x> <y> | info <x> <y> <name>");
    }
    else if (command == "stat") {
        if (tokens.size() != 2) { UI::printError("Usage: stat <happiness|money|life|profession|buildings>"); return true; }
        if (tokens[1] == "buildings") StatCommands::handleStatBuildings(simulation);
        else                     StatCommands::handleStat(simulation, tokens[1]);
    }
    else if (command == "save") {
        if (tokens.size() != 2) { UI::printError("Usage: save <name>"); return true; }
        SimCommands::handleSave(simulation, tokens[1]);
    }
    else if (command == "load") {
        if (tokens.size() != 2) { UI::printError("Usage: load <name>"); return true; }
        SimCommands::handleLoad(simulation, tokens[1]);
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
                UI::printWarning("You have unsaved changes. Save before exit? (y/n):");
                std::string answer;
                std::getline(std::cin, answer);
                if (answer == "y" || answer == "Y") {
                    std::cout << "Enter filename: ";
                    std::string filename;
                    std::getline(std::cin, filename);
                    SimCommands::handleSave(simulation, filename);
                }
            }
            break;
        }
    }
}