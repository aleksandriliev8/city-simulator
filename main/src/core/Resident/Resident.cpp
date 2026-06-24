#include "Resident.hpp"

Resident::Resident(const std::string& name, Profession* profession, int happiness, int money, int life)
    : name(name), profession(profession), happiness(happiness), money(money), life(life) {
}

Resident::~Resident() {
    delete profession;
}

Resident::Resident(const Resident& other)
    : name(other.name), profession(other.profession->clone()), happiness(other.happiness), money(other.money), life(other.life) {
}

Resident& Resident::operator=(const Resident& other) {
    if (this != &other) {
        Profession* newProfession = other.profession->clone();
        delete profession;
        profession = newProfession;
        name = other.name;
        happiness = other.happiness;
        money = other.money;
        life = other.life;
    }
    return *this;
}

const std::string& Resident::getName() const {
    return name;
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

void Resident::applyMonthlyEffects() {
    money += profession->generateSalary();
    profession->applyMonthlyEffect(*this);
}

void Resident::payRent(int rent) {
    if (money >= rent) {
        money -= rent;
    }
    else {
        happiness = happiness - 10 < 0 ? 0 : happiness - 10;
        life = life - 10 < 0 ? 0 : life - 10;
    }
}

void Resident::payFood() {
    if (money >= 50) {
        money -= 50;
    }
    else {
        happiness = happiness - 5 < 0 ? 0 : happiness - 5;
        life = life - 5 < 0 ? 0 : life - 5;
    }
}

bool Resident::isAlive() const {
    return life > 0;
}