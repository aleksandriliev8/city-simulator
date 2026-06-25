#include "Simulation.hpp"
#include "../../io/Serializer/Serializer.hpp"
#include "../../buildings/ModernBuilding/ModernBuilding.hpp"
#include "../../buildings/PanelBuilding/PanelBuilding.hpp"
#include "../../buildings/Dormitory/Dormitory.hpp"
#include "../../professions/Teacher/Teacher.hpp"
#include "../../professions/Programmer/Programmer.hpp"
#include "../../professions/Miner/Miner.hpp"
#include "../../professions/Unemployed/Unemployed.hpp"
#include "../../professions/Student/Student.hpp"
#include "../../utils/Random/Random.hpp"
#include <stdexcept>
#include <fstream>
#include <vector>
#include <string>

static std::string loadRandomCityName() {
    std::ifstream file("assets/city_names.txt");
    if (!file) {
        return "Unknown";
    }

    std::vector<std::string> names;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            names.push_back(line);
        }
    }

    if (names.empty()) {
        return "Unknown";
    }

    return names[Random::randomInt(0, (int)names.size() - 1)];
}

Simulation::Simulation() : city(nullptr), hasUnsavedChanges(false) {
}

Simulation::~Simulation() {
    delete city;
    for (int i = 0; i < (int)snapshots.size(); i++) {
        delete snapshots[i];
    }
}

void Simulation::generate(int rows, int cols) {
    std::string name = loadRandomCityName();

    delete city;
    city = new City(name, rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (Random::randomBool(0.7)) {
                int type = Random::randomInt(0, 2);
                Building* building = nullptr;
                if (type == 0) building = new ModernBuilding(i, j);
                else if (type == 1) building = new PanelBuilding(i, j);
                else building = new Dormitory(i, j);

                int residentCount = Random::randomInt(0, building->getCapacity() / 10);
                for (int r = 0; r < residentCount; r++) {
                    int profType = Random::randomInt(0, 4);
                    Profession* profession = nullptr;
                    if (profType == 0) profession = new Teacher();
                    else if (profType == 1) profession = new Programmer();
                    else if (profType == 2) profession = new Miner();
                    else if (profType == 3) profession = new Unemployed();
                    else profession = new Student();

                    if (profession->getName() == "Student" && !building->canHouseStudent()) {
                        delete profession;
                        profession = new Unemployed();
                    }

                    Resident* resident = new Resident(
                        "Resident_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(r),
                        profession,
                        Random::randomInt(0, 100),
                        Random::randomInt(0, 5000),
                        Random::randomInt(1, 100)
                    );
                    building->addResident(resident);
                }

                city->setBuilding(i, j, building);
            }
        }
    }

    hasUnsavedChanges = true;
}

City* Simulation::findSnapshot(const Date& date) const {
    for (int i = (int)snapshots.size() - 1; i >= 0; i--) {
        if (snapshots[i]->getCurrentDate() == date) {
            return snapshots[i];
        }
    }
    return nullptr;
}

void Simulation::step(int days, int& zeroHappiness, int& zeroLife, int& zeroMoney) {
    if (!hasCity()) {
        throw std::runtime_error("No active simulation");
    }

    Date targetDate = city->getCurrentDate().addDays(days);

    if (targetDate < city->getStartDate()) {
        throw std::invalid_argument("Cannot step before simulation start date");
    }

    zeroHappiness = 0;
    zeroLife = 0;
    zeroMoney = 0;

    if (days < 0) {
        City* snapshot = findSnapshot(targetDate);
        if (snapshot == nullptr) {
            throw std::invalid_argument("No snapshot found for that date");
        }
        delete city;
        city = new City(*snapshot);
    }
    else {
        snapshots.push_back(new City(*city));

        for (int d = 0; d < days; d++) {
            city->advanceDate(1);

            for (int i = 0; i < city->getRows(); i++) {
                for (int j = 0; j < city->getCols(); j++) {
                    Building* building = city->getBuilding(i, j);
                    if (building == nullptr) continue;

                    double rent = building->getRent(city->getRows(), city->getCols());
                    std::vector<std::string> toRemove;

                    for (int k = 0; k < building->getResidentCount(); k++) {
                        Resident* resident = building->getResidents()[k];

                        if (city->getCurrentDate().isFirstOfMonth()) {
                            resident->applyMonthlyEffects(city->getCurrentDate());
                            resident->payRent((int)rent, city->getCurrentDate());
                        }

                        resident->payFood(city->getCurrentDate());

                        if (!resident->isAlive()) {
                            toRemove.push_back(resident->getName());
                        }
                    }

                    for (int r = 0; r < (int)toRemove.size(); r++) {
                        building->removeResident(toRemove[r]);
                    }
                }
            }
        }
    }

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;
            for (int k = 0; k < building->getResidentCount(); k++) {
                Resident* resident = building->getResidents()[k];
                if (resident->getHappiness() == 0) zeroHappiness++;
                if (resident->getLife() == 0)      zeroLife++;
                if (resident->getMoney() == 0)     zeroMoney++;
            }
        }
    }

    hasUnsavedChanges = true;
}

bool Simulation::addResident(int row, int col, const std::string& name, const std::string& job, int happiness, int money, int life) {
    if (!hasCity()) return false;
    if (!city->isValidPosition(row, col)) return false;

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) return false;
    if (building->isFull()) return false;

    Profession* profession = nullptr;
    if (job == "Teacher") profession = new Teacher();
    else if (job == "Programmer") profession = new Programmer();
    else if (job == "Miner") profession = new Miner();
    else if (job == "Unemployed") profession = new Unemployed();
    else if (job == "Student") profession = new Student();
    else return false;

    if (job == "Student" && !building->canHouseStudent()) {
        delete profession;
        return false;
    }

    Resident* resident = new Resident(name, profession, happiness, money, life);
    if (!building->addResident(resident)) {
        delete resident;
        return false;
    }

    hasUnsavedChanges = true;
    return true;
}

bool Simulation::removeResident(int row, int col, const std::string& name) {
    if (!hasCity()) return false;
    if (!city->isValidPosition(row, col)) return false;

    Building* building = city->getBuilding(row, col);
    if (building == nullptr) return false;

    hasUnsavedChanges = true;
    return building->removeResident(name);
}

void Simulation::save(const std::string& filename) {
    Serializer::save(*this, filename);
    hasUnsavedChanges = false;
}

void Simulation::load(const std::string& filename) {
    Serializer::load(*this, filename);
    hasUnsavedChanges = false;
}

City* Simulation::getCity() const {
    return city;
}

bool Simulation::hasCity() const {
    return city != nullptr;
}

void Simulation::setCity(City* city) {
    delete this->city;
    this->city = city;
    hasUnsavedChanges = false;
}

bool Simulation::isUnsaved() const {
    return hasUnsavedChanges;
}