#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "../UI/UI.hpp"
#include "../Paginator/Paginator.hpp"

class StatCommands {
public:
    StatCommands() = delete;

    static void handleStat(Simulation& simulation, const std::string& option);
    static void handleStatBuildings(Simulation& simulation);
};