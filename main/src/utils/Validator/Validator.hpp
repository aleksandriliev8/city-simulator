#pragma once

#include <string>
#include "../../core/City/City.hpp"

namespace Validator {
    bool validateDimensions(int rows, int cols, std::string& error);
    bool validatePosition(const City& city, int row, int col, std::string& error);
    bool validateCharacteristics(int happiness, int money, int life, std::string& error);
    bool validateProfession(const std::string& job, std::string& error);
}