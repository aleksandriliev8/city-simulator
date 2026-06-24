#include "Validator.hpp"

bool Validator::validateDimensions(int rows, int cols, std::string& error) {
    if (rows <= 0 || cols <= 0) {
        error = "Dimensions must be positive.";
        return false;
    }
    return true;
}

bool Validator::validatePosition(const City& city, int row, int col, std::string& error) {
    if (!city.isValidPosition(row, col)) {
        error = "Invalid position (" + std::to_string(row) + ", " + std::to_string(col) + ").";
        return false;
    }
    return true;
}

bool Validator::validateCharacteristics(int happiness, int money, int life, std::string& error) {
    if (happiness < 0 || happiness > 100) {
        error = "Happiness must be between 0 and 100.";
        return false;
    }
    if (money < 0) {
        error = "Money must be non-negative.";
        return false;
    }
    if (life < 0 || life > 100) {
        error = "Life must be between 0 and 100.";
        return false;
    }
    return true;
}

bool Validator::validateProfession(const std::string& job, std::string& error) {
    if (job != "Teacher" && job != "Programmer" && job != "Miner" &&
        job != "Unemployed" && job != "Student") {
        error = "Invalid profession '" + job + "'. Use: Teacher, Programmer, Miner, Unemployed, Student.";
        return false;
    }
    return true;
}