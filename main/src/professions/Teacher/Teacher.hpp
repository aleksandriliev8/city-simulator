#pragma once

#include "../Profession/Profession.hpp"

// Salary: 1200-1300, Monthly effect: happiness +1
class Teacher : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};