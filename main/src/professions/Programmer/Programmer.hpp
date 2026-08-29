#pragma once

#include "../Profession/Profession.hpp"

// Salary: 2000-5000, Monthly effect: happiness -1
class Programmer : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};