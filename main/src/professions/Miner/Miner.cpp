#include "Miner.hpp"
#include "../../core/Resident/Resident.hpp"
#include "../../utils/Random/Random.hpp"

std::string Miner::getName() const {
    return "Miner";
}

int Miner::generateSalary() const {
    return Random::randomInt(1000, 3000);
}

void Miner::applyMonthlyEffect(Resident& resident) {
    int newLife = resident.getLife() - 2;
    resident.setLife(newLife < 0 ? 0 : newLife);
}

Profession* Miner::clone() const {
    return new Miner(*this);
}