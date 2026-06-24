#include "Teacher.hpp"
#include "../../core/Resident/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Teacher::getName() const {
    return "Teacher";
}

int Teacher::generateSalary() const {
    return Random::randomInt(1200, 1300);
}

void Teacher::applyMonthlyEffect(Resident& resident) {
    int newHappiness = resident.getHappiness() + 1;
    resident.setHappiness(newHappiness > 100 ? 100 : newHappiness);
}

Profession* Teacher::clone() const {
    return new Teacher(*this);
}