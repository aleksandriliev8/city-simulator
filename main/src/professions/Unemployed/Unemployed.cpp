#include "Unemployed.hpp"
//#include "../core/Resident.hpp"

std::string Unemployed::getName() const {
    return "Unemployed";
}

int Unemployed::generateSalary() const {
    return 0;
}

void Unemployed::applyMonthlyEffect(Resident& resident) {
    // TODO: resident.setLife(max(resident.getLife() - 1, 0));
    // TODO: resident.setHappiness(max(resident.getHappiness() - 1, 0));
}

Profession* Unemployed::clone() const {
    return new Unemployed(*this);
}