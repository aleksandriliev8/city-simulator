#pragma once

#include <string>
#include <vector>
#include "../../professions/Profession/Profession.hpp"
#include "../HistoryEntry/HistoryEntry.hpp"

// Represents a city resident. Owns its Profession pointer (deep copied via clone()).
// Characteristics (happiness, money, life) are clamped by setters.
// A resident with life == 0 is considered dead and removed during simulation.
class Resident {
private:
    std::string name;
    std::vector<HistoryEntry> history;
    Profession* profession;   // owned pointer, deep-copied in Rule of Three
    int happiness;             // 0-100
    int money;                 // >= 0
    int life;                  // 0-100, death at 0

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

    void applyMonthlyEffects(const Date& date);
    void payRent(int rent, const Date& date);
    void payFood(const Date& date);
    bool isAlive() const;
};