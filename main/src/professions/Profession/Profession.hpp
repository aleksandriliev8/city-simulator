#pragma once

#include <string>

class Resident;

class Profession {
public:
    virtual std::string getName() const = 0;
    virtual int generateSalary() const = 0;
    virtual void applyMonthlyEffect(Resident& resident) = 0;
    virtual Profession* clone() const = 0;
    virtual ~Profession() = default;
};