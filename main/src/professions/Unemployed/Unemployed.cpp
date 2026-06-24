#include "Unemployed.hpp"
#include "../../core/Resident/Resident.hpp"

std::string Unemployed::getName() const {
    return "Unemployed";
}

int Unemployed::generateSalary() const {
    return 0;
}

void Unemployed::applyMonthlyEffect(Resident& resident) {
    int newLife = resident.getLife() - 1;
    resident.setLife(newLife < 0 ? 0 : newLife);

    int newHappiness = resident.getHappiness() - 1;
    resident.setHappiness(newHappiness < 0 ? 0 : newHappiness);
}

Profession* Unemployed::clone() const {
    return new Unemployed(*this);
}