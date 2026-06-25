#include "ModernBuilding.hpp"

ModernBuilding::ModernBuilding(int row, int col) : Building(row, col) {
}

std::string ModernBuilding::getType() const {
    return "Modern";
}

double ModernBuilding::getBaseRent() const {
    return 1000.0;
}

int ModernBuilding::getCapacity() const {
    return 50;
}

bool ModernBuilding::canHouseStudent() const {
    return false;
}

Building* ModernBuilding::clone() const {
    return new ModernBuilding(*this);
}