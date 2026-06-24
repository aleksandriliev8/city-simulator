#include "Simulation.hpp"
#include "../../core/City/City.hpp"
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

Simulation::Simulation() : city(nullptr), hasUnsavedChanges(false) {
}

Simulation::~Simulation() {
    delete city;
    for (int i = 0; i < (int)snapshots.size(); i++) {
        delete snapshots[i];
    }
}

void Simulation::generate(int rows, int cols) {
    // TODO: load city names from assets/city_names.txt
    std::string name = "Sofia";

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
                city->setBuilding(i, j, building);
            }
        }
    }

    hasUnsavedChanges = true;
}

void Simulation::step(int days) {
    if (!hasCity()) {
        throw std::runtime_error("No active simulation");
    }

    if (city->getCurrentDate().addDays(days) < city->getStartDate()) {
        throw std::invalid_argument("Cannot step before simulation start date");
    }

    snapshots.push_back(new City(*city));

    city->advanceDate(days);

    // TODO: apply daily effects to all residents

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

City* Simulation::getCity() const {
    return city;
}

bool Simulation::hasCity() const {
    return city != nullptr;
}

bool Simulation::isUnsaved() const {
    return hasUnsavedChanges;
}

void Simulation::save(const std::string& filename) const {
    // TODO: Serializer
}

void Simulation::load(const std::string& filename) {
    // TODO: Serializer
}