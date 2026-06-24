#include "Student.hpp"
//#include "../core/Resident.hpp"

std::string Student::getName() const {
    return "Student";
}

int Student::generateSalary() const {
    return 0;
}

void Student::applyMonthlyEffect(Resident& resident) {
    // TODO: resident.setHappiness(max(resident.getHappiness() - 1, 0));
}

Profession* Student::clone() const {
    return new Student(*this);
}