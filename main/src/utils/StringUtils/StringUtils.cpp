#include "StringUtils.hpp"

namespace StringUtils {
    std::vector<std::string> tokenize(const std::string& line) {
        std::vector<std::string> tokens;
        std::string token;
        for (int i = 0; i <= (int)line.size(); i++) {
            if (i == (int)line.size() || line[i] == ' ') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            }
            else {
                token += line[i];
            }
        }
        return tokens;
    }

    int toInt(const std::string& s) {
        int result = 0;
        bool negative = false;
        int i = 0;
        if (!s.empty() && s[0] == '-') { negative = true; i = 1; }
        for (; i < (int)s.size(); i++) {
            result = result * 10 + (s[i] - '0');
        }
        return negative ? -result : result;
    }

    bool isInt(const std::string& s) {
        if (s.empty()) return false;
        int i = (s[0] == '-') ? 1 : 0;
        if (i == (int)s.size()) return false;
        for (; i < (int)s.size(); i++) {
            if (s[i] < '0' || s[i] > '9') return false;
        }
        return true;
    }
}