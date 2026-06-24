#include "City.hpp"
#include <stdexcept>

City::City(const std::string& name, int rows, int cols)
    : name(name), rows(rows), cols(cols), startDate(), currentDate() {
    allocateMatrix();
}

City::~City() {
    deallocateMatrix();
}

City::City(const City& other)
    : name(other.name), rows(other.rows), cols(other.cols),
    startDate(other.startDate), currentDate(other.currentDate) {
    allocateMatrix();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (other.matrix[i][j] != nullptr) {
                matrix[i][j] = other.matrix[i][j]->clone();
            }
        }
    }
}

City& City::operator=(const City& other) {
    if (this != &other) {
        std::vector<std::vector<Building*>> newMatrix(other.rows, std::vector<Building*>(other.cols, nullptr));
        for (int i = 0; i < other.rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                if (other.matrix[i][j] != nullptr) {
                    newMatrix[i][j] = other.matrix[i][j]->clone();
                }
            }
        }
        deallocateMatrix();
        matrix = newMatrix;
        name = other.name;
        rows = other.rows;
        cols = other.cols;
        startDate = other.startDate;
        currentDate = other.currentDate;
    }
    return *this;
}

void City::allocateMatrix() {
    matrix = std::vector<std::vector<Building*>>(rows, std::vector<Building*>(cols, nullptr));
}

void City::deallocateMatrix() {
    for (int i = 0; i < (int)matrix.size(); i++) {
        for (int j = 0; j < (int)matrix[i].size(); j++) {
            delete matrix[i][j];
        }
    }
}

const std::string& City::getName() const {
    return name;
}

int City::getRows() const {
    return rows;
}

int City::getCols() const {
    return cols;
}

const Date& City::getStartDate() const {
    return startDate;
}

const Date& City::getCurrentDate() const {
    return currentDate;
}

Building* City::getBuilding(int row, int col) const {
    if (!isValidPosition(row, col)) {
        throw std::invalid_argument("City: invalid position");
    }
    return matrix[row][col];
}

void City::setBuilding(int row, int col, Building* building) {