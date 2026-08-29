#include "StatCommands.hpp"
#include <cmath>

void StatCommands::handleStat(Simulation& simulation, const std::string& option) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    if (option != "happiness" && option != "money" && option != "life" && option != "profession") {
        UI::printError("Invalid option. Use: happiness, money, life, profession.");
        return;
    }

    City* city = simulation.getCity();
    int total = 0, min = 101, max = -1, count = 0;
    int teachers = 0, programmers = 0, miners = 0, unemployed = 0, students = 0;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;
            for (int k = 0; k < building->getResidentCount(); k++) {
                Resident* resident = building->getResidents()[k];
                count++;
                if (option == "profession") {
                    std::string professionName = resident->getProfession()->getName();
                    if (professionName == "Teacher")         teachers++;
                    else if (professionName == "Programmer") programmers++;
                    else if (professionName == "Miner")      miners++;
                    else if (professionName == "Unemployed") unemployed++;
                    else if (professionName == "Student")    students++;
                    continue;
                }
                int value = 0;
                if (option == "happiness")     value = resident->getHappiness();
                else if (option == "money")    value = resident->getMoney();
                else if (option == "life")     value = resident->getLife();
                total += value;
                if (value < min) min = value;
                if (value > max) max = value;
            }
        }
    }

    Paginator paginator;
    paginator.addLine("Total residents: " + std::to_string(count));
    if (option == "profession") {
        paginator.addLine("Teachers:    " + std::to_string(teachers));
        paginator.addLine("Programmers: " + std::to_string(programmers));
        paginator.addLine("Miners:      " + std::to_string(miners));
        paginator.addLine("Unemployed:  " + std::to_string(unemployed));
        paginator.addLine("Students:    " + std::to_string(students));
    }
    else {
        if (count == 0) {
            UI::printError("No residents in simulation.");
            return;
        }
        double avg = (double)total / count;
        std::string avgStr = std::to_string(avg);
        size_t decimalPos = avgStr.find('.');
        if (decimalPos != std::string::npos && decimalPos + 3 < avgStr.size()) avgStr = avgStr.substr(0, decimalPos + 3);
        paginator.addLine("Average: " + avgStr);
        paginator.addLine("Min:     " + std::to_string(min));
        paginator.addLine("Max:     " + std::to_string(max));
    }
    paginator.display();
}

void StatCommands::handleStatBuildings(Simulation& simulation) {
    if (!simulation.hasCity()) {
        UI::printError("No active simulation.");
        return;
    }

    City* city = simulation.getCity();
    int modern = 0, panel = 0, dormitory = 0;
    int central = 0, peripheral = 0, standard = 0;

    double minDimension = (double)(city->getRows() < city->getCols() ? city->getRows() : city->getCols());
    double centerRow = city->getRows() / 2.0;
    double centerCol = city->getCols() / 2.0;

    for (int i = 0; i < city->getRows(); i++) {
        for (int j = 0; j < city->getCols(); j++) {
            Building* building = city->getBuilding(i, j);
            if (building == nullptr) continue;
            std::string type = building->getType();
            if (type == "Modern")         modern++;
            else if (type == "Panel")     panel++;
            else if (type == "Dormitory") dormitory++;
            double distance = std::sqrt((i - centerRow) * (i - centerRow) + (j - centerCol) * (j - centerCol));
            if (distance <= minDimension / 8.0)            central++;
            else if (distance > 6.0 * minDimension / 8.0) peripheral++;
            else                                     standard++;
        }
    }

    Paginator paginator;
    paginator.addLine("--- By type ---");
    paginator.addLine("Modern:     " + std::to_string(modern));
    paginator.addLine("Panel:      " + std::to_string(panel));
    paginator.addLine("Dormitory:  " + std::to_string(dormitory));
    paginator.addLine("--- By location ---");
    paginator.addLine("Central:    " + std::to_string(central));
    paginator.addLine("Standard:   " + std::to_string(standard));
    paginator.addLine("Peripheral: " + std::to_string(peripheral));
    paginator.display();
}