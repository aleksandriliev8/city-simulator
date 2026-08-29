#pragma once

#include "../../core/Building/Building.hpp"

// Rent: 500, Capacity: 100, Students: not allowed
class PanelBuilding : public Building {
public:
    PanelBuilding(int row, int col);

    std::string getType() const override;
    double getBaseRent() const override;
    int getCapacity() const override;
    bool canHouseStudent() const override;
    Building* clone() const override;
};