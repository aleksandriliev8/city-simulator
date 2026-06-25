#pragma once

#include <string>
#include <vector>
#include "../../core/City/City.hpp"

class Simulation {
private:
    City* city;
    std::vector<City*> snapshots;
    bool hasUnsavedChanges;

private:
    City* findSnapshot(const Date& date) const;

public:
    Simulation();
    ~Simulation();
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;

    void generate(int rows, int cols);
    void step(int days, int& zeroHappiness, int& zeroLife, int& zeroMoney);

    bool addResident(int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life);
    bool removeResident(int row, int col, const std::string& name);

    void save(const std::string& filename);
    void load(const std::string& filename);

    City* getCity() const;
    bool hasCity() const;
    void setCity(City* city);
    bool isUnsaved() const;
};