#include "Miner.hpp"
#include "../core/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Miner::getName() const {
    return "Miner";
}

int Miner::generateSalary() const {
    return Random::randomInt(1000, 3000);
}

void Miner::applyMonthlyEffect(Resident& resident) {
    // TODO: resident.setLife(max(resident.getLife() - 2, 0));
}

Profession* Miner::clone() const {
    return new Miner(*this);
}