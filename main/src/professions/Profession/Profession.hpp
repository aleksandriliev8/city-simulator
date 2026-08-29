#pragma once

#include <string>

class Resident;

// Abstract base class for professions. New professions are added by subclassing.
// Each profession defines its own salary range and monthly effect on the resident.
class Profession {
public:
    virtual std::string getName() const = 0;
    virtual int generateSalary() const = 0;
    virtual void applyMonthlyEffect(Resident& resident) = 0;
    virtual Profession* clone() const = 0;
    virtual ~Profession() = default;
};