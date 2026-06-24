#include "PanelBuilding.hpp"

PanelBuilding::PanelBuilding(int row, int col) : Building(row, col) {
}

std::string PanelBuilding::getType() const {
    return "Panel";
}

double PanelBuilding::getBaseRent() const {
    return 500.0;
}

int PanelBuilding::getCapacity() const {
    return 100;
}

bool PanelBuilding::canHouseStudent() const {
    return false;
}

Building* PanelBuilding::clone() const {
    return new PanelBuilding(*this);
}