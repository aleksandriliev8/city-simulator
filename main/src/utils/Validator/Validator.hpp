#pragma once

#include <string>
#include "../../core/City/City.hpp"

class Validator {
public:
    Validator() = delete;

    static bool validateDimensions(int rows, int cols, std::string& error);
    static bool validatePosition(const City& city, int row, int col, std::string& error);
    static bool validateCharacteristics(int happiness, int money, int life, std::string& error);
    static bool validateProfession(const std::string& job, std::string& error);
};