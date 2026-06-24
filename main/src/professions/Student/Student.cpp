#include "Student.hpp"
#include "../../core/Resident/Resident.hpp"

std::string Student::getName() const {
    return "Student";
}

int Student::generateSalary() const {
    return 0;
}

void Student::applyMonthlyEffect(Resident& resident) {
    int newHappiness = resident.getHappiness() - 1;
    resident.setHappiness(newHappiness < 0 ? 0 : newHappiness);
}

Profession* Student::clone() const {
    return new Student(*this);
}