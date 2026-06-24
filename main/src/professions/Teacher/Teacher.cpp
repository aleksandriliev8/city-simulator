#include "Teacher.hpp"
#include "../core/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Teacher::getName() const {
    return "Teacher";
}

int Teacher::generateSalary() const {
    return Random::randomInt(1200, 1300);
}

void Teacher::applyMonthlyEffect(Resident& resident) {
    // TODO: resident.setHappiness(min(resident.getHappiness() + 1, 100));
}

Profession* Teacher::clone() const {
    return new Teacher(*this);
}