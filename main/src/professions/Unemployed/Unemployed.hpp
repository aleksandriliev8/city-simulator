#pragma once

#include "../Profession/Profession.hpp"

// Salary: 0, Monthly effect: life -1, happiness -1
class Unemployed : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};