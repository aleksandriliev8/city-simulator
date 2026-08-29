#pragma once

#include <string>
#include <vector>
#include "../Resident/Resident.hpp"

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

    virtual std::string getType() const = 0;
    virtual double getBaseRent() const = 0;
    virtual int getCapacity() const = 0;
    virtual bool canHouseStudent() const = 0;
    virtual Building* clone() const = 0;

    int getRow() const;
    int getCol() const;
    double getRent(int gridRows, int gridCols) const;

    bool addResident(Resident* resident);
    bool removeResident(const std::string& name);
    Resident* getResident(const std::string& name) const;
    const std::vector<Resident*>& getResidents() const;
    int getResidentCount() const;
    int getFreeSlots() const;
    bool isFull() const;
};