#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "../UI/UI.hpp"
#include "../../utils/Validator/Validator.hpp"

class SimCommands {
public:
    SimCommands() = delete;

    static void handleGenerate(Simulation& simulation, int rows, int cols);
    static void handleAdd(Simulation& simulation, int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life);
    static void handleRemove(Simulation& simulation, int row, int col, const std::string& name);
    static void handleStep(Simulation& simulation, int days);
    static void handleSave(Simulation& simulation, const std::string& filename);
    static void handleLoad(Simulation& simulation, const std::string& filename);
};