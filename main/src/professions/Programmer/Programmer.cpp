#include "Programmer.hpp"
//#include "../core/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Programmer::getName() const {
    return "Programmer";
}

int Programmer::generateSalary() const {
    return Random::randomInt(2000, 5000);
}

void Programmer::applyMonthlyEffect(Resident& resident) {
    // TODO: resident.setHappiness(max(resident.getHappiness() - 1, 0));
}

Profession* Programmer::clone() const {
    return new Programmer(*this);
}