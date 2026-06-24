#include "Programmer.hpp"
#include "../../core/Resident/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Programmer::getName() const {
    return "Programmer";
}

int Programmer::generateSalary() const {
    return Random::randomInt(2000, 5000);
}

void Programmer::applyMonthlyEffect(Resident& resident) {
    int newHappiness = resident.getHappiness() - 1;
    resident.setHappiness(newHappiness < 0 ? 0 : newHappiness);
}
Profession* Programmer::clone() const {
    return new Programmer(*this);
}