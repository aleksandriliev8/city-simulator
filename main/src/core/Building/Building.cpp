#include "Building.hpp"
#include <cmath>
#include <stdexcept>

Building::Building(int row, int col) : row(row), col(col) {
}

Building::~Building() {
    for (int i = 0; i < (int)residents.size(); i++) {
        delete residents[i];
    }
}

Building::Building(const Building& other) : row(other.row), col(other.col) {
    for (int i = 0; i < (int)other.residents.size(); i++) {
        residents.push_back(new Resident(*other.residents[i]));
    }
}

Building& Building::operator=(const Building& other) {
    if (this != &other) {
        std::vector<Resident*> newResidents;
        for (int i = 0; i < (int)other.residents.size(); i++) {
            newResidents.push_back(new Resident(*other.residents[i]));
        }
        for (int i = 0; i < (int)residents.size(); i++) {
            delete residents[i];
        }
        residents = newResidents;
        row = other.row;
        col = other.col;
    }
    return *this;
}

int Building::getRow() const {
    return row;
}

int Building::getCol() const {
    return col;
}

// Location-based rent: Euclidean distance from grid center determines the zone.
// Central (dist <= min(n,m)/8): x2.5, Peripheral (dist > 6*min(n,m)/8): x0.8, Standard: x1.0
double Building::getRent(int gridRows, int gridCols) const {
    double centerRow = gridRows / 2.0;
    double centerCol = gridCols / 2.0;
    double distance = std::sqrt((row - centerRow) * (row - centerRow) + (col - centerCol) * (col - centerCol));
    double minDimension = (double)(gridRows < gridCols ? gridRows : gridCols);

    if (distance <= minDimension / 8.0) {
        return getBaseRent() * 2.5;
    }
    else if (distance > 6.0 * minDimension / 8.0) {
        return getBaseRent() * 0.8;
    }
    return getBaseRent();
}

bool Building::addResident(Resident* resident) {
    if (isFull()) {
        return false;
    }
    if (getResident(resident->getName()) != nullptr) {
        return false;
    }
    residents.push_back(resident);
    return true;
}

bool Building::removeResident(const std::string& name) {
    for (int i = 0; i < (int)residents.size(); i++) {
        if (residents[i]->getName() == name) {
            delete residents[i];
            residents.erase(residents.begin() + i);
            return true;
        }
    }
    return false;
}

Resident* Building::getResident(const std::string& name) const {
    for (int i = 0; i < (int)residents.size(); i++) {
        if (residents[i]->getName() == name) {
            return residents[i];
        }
    }
    return nullptr;
}

const std::vector<Resident*>& Building::getResidents() const {
    return residents;
}

int Building::getResidentCount() const {
    return (int)residents.size();
}

int Building::getFreeSlots() const {
    return getCapacity() - getResidentCount();
}

bool Building::isFull() const {
    return getResidentCount() >= getCapacity();
}