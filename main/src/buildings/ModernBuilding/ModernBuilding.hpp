#pragma once

#include "../../core/Building/Building.hpp"

class ModernBuilding : public Building {
public:
    ModernBuilding(int row, int col);

    std::string getType() const override;
    double getBaseRent() const override;
    int getCapacity() const override;
    bool canHouseStudent() const override;
    Building* clone() const override;
};