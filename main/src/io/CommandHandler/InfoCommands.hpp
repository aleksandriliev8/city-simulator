#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "../UI/UI.hpp"
#include "../Paginator/Paginator.hpp"
#include "../../utils/Validator/Validator.hpp"

class InfoCommands {
public:
    InfoCommands() = delete;

    static void handleInfo(Simulation& simulation);
    static void handleInfo(Simulation& simulation, int row, int col);
    static void handleInfo(Simulation& simulation, int row, int col, const std::string& name);
};