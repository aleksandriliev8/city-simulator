#pragma once

#include <string>
#include <vector>
#include "../Building/Building.hpp"
#include "../HistoryEntry/HistoryEntry.hpp"
#include "../../utils/Date/Date.hpp"

// Represents the city as a 2D matrix of Building pointers.
// Owns all Building objects. Implements Rule of Three with deep copy
// via Building::clone(). Tracks simulation dates and event history.
class City {
private:
    std::string name;
    std::vector<std::vector<Building*>> matrix;  // rows x cols grid of buildings (nullptr = empty cell)
    std::vector<HistoryEntry> history;
    Date startDate;
    Date currentDate;
    int rows;
    int cols;

    void allocateMatrix();
    void deallocateMatrix();

public:
    City(const std::string& name, int rows, int cols);
    City(const std::string& name, int rows, int cols, const Date& startDate, const Date& currentDate);
    ~City();
    City(const City& other);
    City& operator=(const City& other);

    const std::string& getName() const;
    int getRows() const;
    int getCols() const;
    const Date& getStartDate() const;
    const Date& getCurrentDate() const;
    const std::vector<HistoryEntry>& getHistory() const;

    Building* getBuilding(int row, int col) const;
    void setBuilding(int row, int col, Building* building);
    bool isValidPosition(int row, int col) const;

    void advanceDate(int days);
    void addHistoryEntry(const std::string& description);
    void addHistoryEntry(const Date& date, const std::string& description);
};