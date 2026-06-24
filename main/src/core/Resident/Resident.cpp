#include "Resident.hpp"

Resident::Resident(const std::string& name, Profession* profession, int happiness, int money, int life)
    : name(name), history(), profession(profession), happiness(happiness), money(money), life(life) {
}

Resident::~Resident() {
    delete profession;
}

Resident::Resident(const Resident& other)
    : name(other.name), history(other.history), profession(other.profession->clone()),
      happiness(other.happiness), money(other.money), life(other.life) {
}

Resident& Resident::operator=(const Resident& other) {
    if (this != &other) {
        Profession* newProfession = other.profession->clone();
        delete profession;
        profession = newProfession;
        name = other.name;
        history = other.history;
        happiness = other.happiness;
        money = other.money;
        life = other.life;
    }
    return *this;
}

const std::string& Resident::getName() const {
    return name;
}

const std::vector<HistoryEntry>& Resident::getHistory() const {
    return history;
}

Profession* Resident::getProfession() const {
    return profession;
}

int Resident::getHappiness() const {
    return happiness;
}

int Resident::getMoney() const {
    return money;
}

int Resident::getLife() const {
    return life;
}

void Resident::setHappiness(int happiness) {
    if (happiness < 0) happiness = 0;
    if (happiness > 100) happiness = 100;
    this->happiness = happiness;
}

void Resident::setMoney(int money) {
    if (money < 0) money = 0;
    this->money = money;
}

void Resident::setLife(int life) {
    if (life < 0) life = 0;
    if (life > 100) life = 100;
    this->life = life;
}

void Resident::addHistoryEntry(const Date& date, const std::string& description) {
    history.push_back(HistoryEntry(date, description));
}

void Resident::applyMonthlyEffects() {
    money += profession->generateSalary();
    profession->applyMonthlyEffect(*this);
}

void Resident::payRent(int rent, const Date& date) {
    if (money >= rent) {
        money -= rent;
        addHistoryEntry(date, "Paid rent: " + std::to_string(rent) + " EUR");
    }
    else {
        happiness = happiness - 10 < 0 ? 0 : happiness - 10;
        life = life - 10 < 0 ? 0 : life - 10;
        addHistoryEntry(date, "Could not pay rent");
    }
}

void Resident::payFood(const Date& date) {
    if (money >= 50) {
        money -= 50;
        addHistoryEntry(date, "Paid food: 50 EUR");
    }
    else {
        happiness = happiness - 5 < 0 ? 0 : happiness - 5;
        life = life - 5 < 0 ? 0 : life - 5;
        addHistoryEntry(date, "Could not pay food");
    }
}

bool Resident::isAlive() const {
    return life > 0;
}