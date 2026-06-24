#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"
#include "../UI/UI.hpp"

class CommandHandler {
private:
    Simulation& simulation;

    void handleGenerate(int rows, int cols);
    void handleAdd(int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life);
    void handleRemove(int row, int col, const std::string& name);
    void handleStep(int days);
    void handleInfo();
    void handleInfo(int row, int col);
    void handleInfo(int row, int col, const std::string& name);
    void handleStat(const std::string& option);
    void handleStatBuildings();
    void handleSave(const std::string& filename);
    void handleLoad(const std::string& filename);

public:
    CommandHandler(Simulation& simulation);

    void run();
    bool parseAndExecute(const std::string& line);
};