#pragma once

#include <string>
#include "../simulation/Simulation.hpp"

class Serializer {
public:
    Serializer() = delete;

    static void save(const Simulation& simulation, const std::string& filename);
    static void load(Simulation& simulation, const std::string& filename);
};