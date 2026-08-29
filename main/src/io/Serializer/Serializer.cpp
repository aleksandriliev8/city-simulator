#include "Serializer.hpp"
#include "../../core/City/City.hpp"
#include "../../core/Building/Building.hpp"
#include "../../core/Resident/Resident.hpp"
#include "../../buildings/ModernBuilding/ModernBuilding.hpp"
#include "../../buildings/PanelBuilding/PanelBuilding.hpp"
#include "../../buildings/Dormitory/Dormitory.hpp"
#include "../../professions/Teacher/Teacher.hpp"
#include "../../professions/Programmer/Programmer.hpp"
#include "../../professions/Miner/Miner.hpp"
#include "../../professions/Unemployed/Unemployed.hpp"
#include "../../professions/Student/Student.hpp"
#include <fstream>
#include <stdexcept>

// Resolves the data directory path regardless of working directory
static std::string resolveDataPath(const std::string& filename) {
    std::string paths[] = {
        "data/" + filename,
        "../main/data/" + filename,
        "main/data/" + filename
    };
    for (int i = 0; i < 3; i++) {
        std::ifstream test(paths[i], std::ios::binary);
        if (test.good()) return paths[i];
    }
    return paths[0];
}

static std::string resolveDataDir() {
    std::string dirs[] = { "data/", "../main/data/", "main/data/" };
    for (int i = 0; i < 3; i++) {
        std::ifstream test(dirs[i] + ".gitkeep");
        if (test.good()) return dirs[i];
    }
    return dirs[0];
}

void Serializer::save(const Simulation& simulation, const std::string& filename) {
    std::ofstream file(resolveDataDir() + filename + ".dat", std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    City* city = simulation.getCity();

    // write city name
    int cityNameLen = (int)city->getName().size();
    file.write((char*)&cityNameLen, sizeof(int));
    file.write(city->getName().c_str(), cityNameLen);

    // write dimensions
    int rows = city->getRows();
    int cols = city->getCols();
    file.write((char*)&rows, sizeof(int));
    file.write((char*)&cols, sizeof(int));

    // write start date
    int day = city->getStartDate().getDay();
    int month = city->getStartDate().getMonth();
    int year = city->getStartDate().getYear();
    file.write((char*)&day, sizeof(int));
    file.write((char*)&month, sizeof(int));
    file.write((char*)&year, sizeof(int));

    // write current date
    day = city->getCurrentDate().getDay();
    month = city->getCurrentDate().getMonth();
    year = city->getCurrentDate().getYear();
    file.write((char*)&day, sizeof(int));
    file.write((char*)&month, sizeof(int));
    file.write((char*)&year, sizeof(int));

    // write matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) {
                int emptyMarker = -1;
                file.write((char*)&emptyMarker, sizeof(int));
                continue;
            }

            // write building type
            std::string buildingType = building->getType();
            int buildingTypeLen = (int)buildingType.size();
            file.write((char*)&buildingTypeLen, sizeof(int));
            file.write(buildingType.c_str(), buildingTypeLen);

            // write residents
            int residentCount = building->getResidentCount();
            file.write((char*)&residentCount, sizeof(int));

            for (int k = 0; k < residentCount; k++) {
                Resident* resident = building->getResidents()[k];

                // write name
                int residentNameLen = (int)resident->getName().size();
                file.write((char*)&residentNameLen, sizeof(int));
                file.write(resident->getName().c_str(), residentNameLen);

                // write profession
                std::string professionName = resident->getProfession()->getName();
                int professionNameLen = (int)professionName.size();
                file.write((char*)&professionNameLen, sizeof(int));
                file.write(professionName.c_str(), professionNameLen);

                // write characteristics
                int happiness = resident->getHappiness();
                int money = resident->getMoney();
                int life = resident->getLife();
                file.write((char*)&happiness, sizeof(int));
                file.write((char*)&money, sizeof(int));
                file.write((char*)&life, sizeof(int));

                // write history
                int historySize = (int)resident->getHistory().size();
                file.write((char*)&historySize, sizeof(int));
                for (int h = 0; h < historySize; h++) {
                    const HistoryEntry& entry = resident->getHistory()[h];
                    int entryDay = entry.getDate().getDay();
                    int entryMonth = entry.getDate().getMonth();
                    int entryYear = entry.getDate().getYear();
                    file.write((char*)&entryDay, sizeof(int));
                    file.write((char*)&entryMonth, sizeof(int));
                    file.write((char*)&entryYear, sizeof(int));
                    std::string description = entry.getDescription();
                    int descriptionLen = (int)description.size();
                    file.write((char*)&descriptionLen, sizeof(int));
                    file.write(description.c_str(), descriptionLen);
                }
            }
        }
    }

    // write city history
    int cityHistorySize = (int)city->getHistory().size();
    file.write((char*)&cityHistorySize, sizeof(int));
    for (int h = 0; h < cityHistorySize; h++) {
        const HistoryEntry& entry = city->getHistory()[h];
        int entryDay = entry.getDate().getDay();
        int entryMonth = entry.getDate().getMonth();
        int entryYear = entry.getDate().getYear();
        file.write((char*)&entryDay, sizeof(int));
        file.write((char*)&entryMonth, sizeof(int));
        file.write((char*)&entryYear, sizeof(int));
        std::string description = entry.getDescription();
        int descriptionLen = (int)description.size();
        file.write((char*)&descriptionLen, sizeof(int));
        file.write(description.c_str(), descriptionLen);
    }

    if (!file) {
        throw std::runtime_error("Error writing to file: " + filename);
    }
}

void Serializer::load(Simulation& simulation, const std::string& filename) {
    std::string path = resolveDataPath(filename + ".dat");
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Guard against corrupted/malicious files: all string lengths must be in [0, 10000]
    auto validateLen = [&](int len, const std::string& what) {
        if (len < 0 || len > 10000) {
            throw std::runtime_error("Corrupted file: invalid " + what + " length in " + filename);
        }
    };

    // read city name
    int cityNameLen;
    file.read((char*)&cityNameLen, sizeof(int));
    validateLen(cityNameLen, "city name");
    std::string name(cityNameLen, '\0');
    file.read(&name[0], cityNameLen);

    // read dimensions
    int rows, cols;
    file.read((char*)&rows, sizeof(int));
    file.read((char*)&cols, sizeof(int));

    if (rows <= 0 || cols <= 0) {
        throw std::runtime_error("Invalid city dimensions in file: " + filename);
    }

    // read start date
    int startDay, startMonth, startYear;
    file.read((char*)&startDay, sizeof(int));
    file.read((char*)&startMonth, sizeof(int));
    file.read((char*)&startYear, sizeof(int));

    // read current date
    int curDay, curMonth, curYear;
    file.read((char*)&curDay, sizeof(int));
    file.read((char*)&curMonth, sizeof(int));
    file.read((char*)&curYear, sizeof(int));

    City* city = new City(name, rows, cols, Date(startDay, startMonth, startYear), Date(curDay, curMonth, curYear));

    try {
        // read matrix
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int buildingTypeLen;
                file.read((char*)&buildingTypeLen, sizeof(int));
                if (buildingTypeLen == -1) continue;
                validateLen(buildingTypeLen, "building type");

                std::string buildingType(buildingTypeLen, '\0');
                file.read(&buildingType[0], buildingTypeLen);

                Building* building = nullptr;
                if (buildingType == "Modern")         building = new ModernBuilding(i, j);
                else if (buildingType == "Panel")     building = new PanelBuilding(i, j);
                else if (buildingType == "Dormitory") building = new Dormitory(i, j);
                else throw std::runtime_error("Unknown building type in file: " + filename);

                city->setBuilding(i, j, building);

                int residentCount;
                file.read((char*)&residentCount, sizeof(int));
                if (residentCount < 0 || residentCount > 10000) {
                    throw std::runtime_error("Corrupted file: invalid resident count in " + filename);
                }

                for (int k = 0; k < residentCount; k++) {
                    // read name
                    int residentNameLen;
                    file.read((char*)&residentNameLen, sizeof(int));
                    validateLen(residentNameLen, "resident name");
                    std::string residentName(residentNameLen, '\0');
                    file.read(&residentName[0], residentNameLen);

                    // read profession
                    int professionNameLen;
                    file.read((char*)&professionNameLen, sizeof(int));
                    validateLen(professionNameLen, "profession name");
                    std::string professionName(professionNameLen, '\0');
                    file.read(&professionName[0], professionNameLen);

                    Profession* profession = nullptr;
                    if (professionName == "Teacher")         profession = new Teacher();
                    else if (professionName == "Programmer") profession = new Programmer();
                    else if (professionName == "Miner")      profession = new Miner();
                    else if (professionName == "Unemployed") profession = new Unemployed();
                    else if (professionName == "Student")    profession = new Student();
                    else throw std::runtime_error("Unknown profession in file: " + filename);

                    // read characteristics
                    int happiness, money, life;
                    file.read((char*)&happiness, sizeof(int));
                    file.read((char*)&money, sizeof(int));
                    file.read((char*)&life, sizeof(int));

                    // Constructor clamps values; re-apply via setters to restore exact saved values
                    Resident* resident = new Resident(residentName, profession, happiness, money, life);
                    resident->setHappiness(happiness);
                    resident->setMoney(money);
                    resident->setLife(life);

                    // read history
                    int historySize;
                    file.read((char*)&historySize, sizeof(int));
                    if (historySize < 0 || historySize > 100000) {
                        delete resident;
                        throw std::runtime_error("Corrupted file: invalid history size in " + filename);
                    }
                    for (int h = 0; h < historySize; h++) {
                        int entryDay, entryMonth, entryYear;
                        file.read((char*)&entryDay, sizeof(int));
                        file.read((char*)&entryMonth, sizeof(int));
                        file.read((char*)&entryYear, sizeof(int));
                        int descriptionLen;
                        file.read((char*)&descriptionLen, sizeof(int));
                        validateLen(descriptionLen, "history description");
                        std::string description(descriptionLen, '\0');
                        file.read(&description[0], descriptionLen);
                        resident->addHistoryEntry(Date(entryDay, entryMonth, entryYear), description);
                    }

                    building->addResident(resident);
                }
            }
        }

        // read city history (optional — old save files may not have it)
        int cityHistorySize = 0;
        file.read((char*)&cityHistorySize, sizeof(int));
        if (file.good() && cityHistorySize > 0 && cityHistorySize < 100000) {
            for (int h = 0; h < cityHistorySize; h++) {
                int entryDay, entryMonth, entryYear;
                file.read((char*)&entryDay, sizeof(int));
                file.read((char*)&entryMonth, sizeof(int));
                file.read((char*)&entryYear, sizeof(int));
                int descriptionLen;
                file.read((char*)&descriptionLen, sizeof(int));
                if (descriptionLen < 0 || descriptionLen > 10000) break;
                std::string description(descriptionLen, '\0');
                file.read(&description[0], descriptionLen);
                if (!file.good()) break;
                city->addHistoryEntry(Date(entryDay, entryMonth, entryYear), description);
            }
        }
        file.clear();

        simulation.setCity(city);
    }
    catch (...) {
        delete city;
        throw;
    }
}