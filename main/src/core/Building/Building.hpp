#pragma once

#include <string>
#include <vector>
#include "../Resident/Resident.hpp"

// Abstract base class for all building types (Modern, Panel, Dormitory).
// Owns its Resident pointers. Implements Rule of Three for deep copy.
// Subclasses define type-specific properties via pure virtual methods;
// clone() enables polymorphic copying (used by City deep copy).
class Building {
private:
    int row;
    int col;
    std::vector<Resident*> residents;

public:
    Building(int row, int col);
    virtual ~Building();
    Building(const Building& other);
    Building& operator=(const Building& other);

    // Pure virtuals — each building type provides its own values
    virtual std::string getType() const = 0;
    virtual double getBaseRent() const = 0;
    virtual int getCapacity() const = 0;
    virtual bool canHouseStudent() const = 0;
    virtual Building* clone() const = 0;

    int getRow() const;
    int getCol() const;

    // Calculates rent based on Euclidean distance from city center
    double getRent(int gridRows, int gridCols) const;

    bool addResident(Resident* resident);
    bool removeResident(const std::string& name);
    Resident* getResident(const std::string& name) const;
    const std::vector<Resident*>& getResidents() const;
    int getResidentCount() const;
    int getFreeSlots() const;
    bool isFull() const;
};