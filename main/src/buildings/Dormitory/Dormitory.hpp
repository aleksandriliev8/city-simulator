#pragma once

#include "../../core/Building/Building.hpp"

// Rent: 67, Capacity: 200, Students: only students allowed
class Dormitory : public Building {
public:
    Dormitory(int row, int col);

    std::string getType() const override;
    double getBaseRent() const override;
    int getCapacity() const override;
    bool canHouseStudent() const override;
    Building* clone() const override;
};