#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "SimCommands.hpp"
#include "InfoCommands.hpp"
#include "StatCommands.hpp"
#include "../UI/UI.hpp"

class CommandHandler {
private:
    Simulation& simulation;

public:
    CommandHandler(Simulation& simulation);

    void run();
    bool parseAndExecute(const std::string& line);
};