#pragma once

#include "../Profession/Profession.hpp"

class Unemployed : public Profession {
public:
    std::string getName() const override;
    int generateSalary() const override;
    void applyMonthlyEffect(Resident& resident) override;
    Profession* clone() const override;
};