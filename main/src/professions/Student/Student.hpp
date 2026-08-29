#pragma once

#include "../Profession/Profession.hpp"

// Salary: 0, Monthly effect: happiness -1. Can only live in Dormitory.
class Student : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};