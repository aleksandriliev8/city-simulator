#pragma once

#include "../Profession/Profession.hpp"

// Salary: 1000-3000, Monthly effect: life -2
class Miner : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};