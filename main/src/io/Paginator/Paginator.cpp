#include "Paginator.hpp"
#include <iostream>

Paginator::Paginator(int pageSize) : pageSize(pageSize) {
}

void Paginator::addLine(const std::string& line) {
    lines.push_back(line);
}

void Paginator::clear() {
    lines.clear();
}

void Paginator::display() const {
    int total = (int)lines.size();
    int current = 0;

    while (current < total) {
        for (int i = current; i < current + pageSize && i < total; i++) {
            std::cout << lines[i] << std::endl;
        }
        current += pageSize;

        if (current < total) {
            std::cout << "--- Press Enter for next page, q to quit ---" << std::endl;
            std::string input;
            std::getline(std::cin, input);
            if (input == "q" || input == "Q") {
                break;
            }
        }
    }
}