#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "SimCommands.hpp"
#include "InfoCommands.hpp"
#include "StatCommands.hpp"
#include "../UI/UI.hpp"

// Main REPL controller. Parses user commands and dispatches to the appropriate handler.
// Handles auto-load on startup and auto-save on exit.
class CommandHandler {
private:
    Simulation& simulation;

public:
    CommandHandler(Simulation& simulation);

    void run();
    bool parseAndExecute(const std::string& line);  // returns false on "exit"
};