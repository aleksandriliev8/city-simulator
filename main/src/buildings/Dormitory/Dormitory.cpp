#include "Dormitory.hpp"

Dormitory::Dormitory(int row, int col) : Building(row, col) {
}

std::string Dormitory::getType() const {
    return "Dormitory";
}

double Dormitory::getBaseRent() const {
    return 67.0;
}

int Dormitory::getCapacity() const {
    return 200;
}

bool Dormitory::canHouseStudent() const {
    return true;
}

Building* Dormitory::clone() const {
    return new Dormitory(*this);
}