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

void Serializer::save(const Simulation& simulation, const std::string& filename) {
    std::ofstream file("data/" + filename + ".bin", std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    City* city = simulation.getCity();

    // write city name
    int nameLen = (int)city->getName().size();
    file.write((char*)&nameLen, sizeof(int));
    file.write(city->getName().c_str(), nameLen);

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
                int type = -1;
                file.write((char*)&type, sizeof(int));
                continue;
            }

            // write building type
            std::string type = building->getType();
            int typeLen = (int)type.size();
            file.write((char*)&typeLen, sizeof(int));
            file.write(type.c_str(), typeLen);

            // write residents
            int residentCount = building->getResidentCount();
            file.write((char*)&residentCount, sizeof(int));

            for (int k = 0; k < residentCount; k++) {
                Resident* resident = building->getResidents()[k];

                // write name
                int resNameLen = (int)resident->getName().size();
                file.write((char*)&resNameLen, sizeof(int));
                file.write(resident->getName().c_str(), resNameLen);

                // write profession
                std::string profName = resident->getProfession()->getName();
                int profNameLen = (int)profName.size();
                file.write((char*)&profNameLen, sizeof(int));
                file.write(profName.c_str(), profNameLen);

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
                    std::string desc = entry.getDescription();
                    int descLen = (int)desc.size();
                    file.write((char*)&descLen, sizeof(int));
                    file.write(desc.c_str(), descLen);
                }
            }
        }
    }

    if (!file) {
        throw std::runtime_error("Error writing to file: " + filename);
    }
}

void Serializer::load(Simulation& simulation, const std::string& filename) {
    std::ifstream file("data/" + filename + ".bin", std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // read city name
    int nameLen;
    file.read((char*)&nameLen, sizeof(int));
    std::string name(nameLen, '\0');
    file.read(&name[0], nameLen);

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
simulation.setCity(city);

    // read matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int typeLen;
            file.read((char*)&typeLen, sizeof(int));
            if (typeLen == -1) {
                continue;
            }

            std::string type(typeLen, '\0');
            file.read(&type[0], typeLen);

            Building* building = nullptr;
            if (type == "Modern") building = new ModernBuilding(i, j);
            else if (type == "Panel") building = new PanelBuilding(i, j);
            else if (type == "Dormitory") building = new Dormitory(i, j);
            else throw std::runtime_error("Unknown building type in file: " + filename);

            city->setBuilding(i, j, building);

            int residentCount;
            file.read((char*)&residentCount, sizeof(int));

            for (int k = 0; k < residentCount; k++) {
                // read name
                int resNameLen;
                file.read((char*)&resNameLen, sizeof(int));
                std::string resName(resNameLen, '\0');
                file.read(&resName[0], resNameLen);

                // read profession
                int profNameLen;
                file.read((char*)&profNameLen, sizeof(int));
                std::string profName(profNameLen, '\0');
                file.read(&profName[0], profNameLen);

                Profession* profession = nullptr;
                if (profName == "Teacher") profession = new Teacher();
                else if (profName == "Programmer") profession = new Programmer();
                else if (profName == "Miner") profession = new Miner();
                else if (profName == "Unemployed") profession = new Unemployed();
                else if (profName == "Student") profession = new Student();
                else throw std::runtime_error("Unknown profession in file: " + filename);

                // read characteristics
                int happiness, money, life;
                file.read((char*)&happiness, sizeof(int));
                file.read((char*)&money, sizeof(int));
                file.read((char*)&life, sizeof(int));

                Resident* resident = new Resident(resName, profession, happiness, money, life);

                // read history
                int historySize;
                file.read((char*)&historySize, sizeof(int));
                for (int h = 0; h < historySize; h++) {
                    int entryDay, entryMonth, entryYear;
                    file.read((char*)&entryDay, sizeof(int));
                    file.read((char*)&entryMonth, sizeof(int));
                    file.read((char*)&entryYear, sizeof(int));
                    int descLen;
                    file.read((char*)&descLen, sizeof(int));
                    std::string desc(descLen, '\0');
                    file.read(&desc[0], descLen);
                    resident->addHistoryEntry(Date(entryDay, entryMonth, entryYear), desc);
                }

                building->addResident(resident);
            }
        }
    }

    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }
}