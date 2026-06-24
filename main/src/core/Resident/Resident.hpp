#pragma once

#include <string>
#include <vector>
#include "../../professions/Profession/Profession.hpp"
#include "../HistoryEntry/HistoryEntry.hpp"

class Resident {
private:
    std::string name;
    std::vector<HistoryEntry> history;
    Profession* profession;
    int happiness;
    int money;
    int life;

public:
    Resident(const std::string& name, Profession* profession, int happiness, int money, int life);
    ~Resident();
    Resident(const Resident& other);
    Resident& operator=(const Resident& other);

    const std::string& getName() const;
    const std::vector<HistoryEntry>& getHistory() const;
    Profession* getProfession() const;
    int getHappiness() const;
    int getMoney() const;
    int getLife() const;

    void setHappiness(int happiness);
    void setMoney(int money);
    void setLife(int life);

    void addHistoryEntry(const Date& date, const std::string& description);

    void applyMonthlyEffects();
    void payRent(int rent, const Date& date);
    void payFood(const Date& date);
    bool isAlive() const;
};