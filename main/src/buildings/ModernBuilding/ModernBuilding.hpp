#pragma once

#include "../../core/Building/Building.hpp"

// Rent: 1000, Capacity: 50, Students: not allowed
class ModernBuilding : public Building {
public:
    ModernBuilding(int row, int col);

    std::string getType() const override;
    double getBaseRent() const override;
    int getCapacity() const override;
    bool canHouseStudent() const override;
    Building* clone() const override;
};