#pragma once

#include <string>
#include <vector>

namespace StringUtils {
    std::vector<std::string> tokenize(const std::string& line);
    int toInt(const std::string& s);
    bool isInt(const std::string& s);
}