#pragma once

#include <vector>
#include <string>

class Paginator {
private:
    std::vector<std::string> lines;
    int pageSize;

public:
    Paginator(int pageSize = 20);

    void addLine(const std::string& line);
    void clear();
    void display() const;
};