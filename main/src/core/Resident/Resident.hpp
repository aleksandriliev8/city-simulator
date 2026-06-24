#pragma once

#include <string>
#include "../../professions/Profession/Profession.hpp"

class Resident {
private:
    std::string name;
    Profession* profession;
    int happiness;
    int money;
    int life;
    // TODO: History history;

public:
    Resident(const std::string& name, Profession* profession, int happiness, int money, int life);
    ~Resident();
    Resident(const Resident& other);
    Resident& operator=(const Resident& other);

    const std::string& getName() const;
    Profession* getProfession() const;
    int getHappiness() const;
    int getMoney() const;
    int getLife() const;

    void setHappiness(int happiness);
    void setMoney(int money);
    void setLife(int life);

    void applyMonthlyEffects();
    void payRent(int rent);
    void payFood();
    bool isAlive() const;
};