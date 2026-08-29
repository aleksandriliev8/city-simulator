#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"

namespace Serializer {
    void save(const Simulation& simulation, const std::string& filename);
    void load(Simulation& simulation, const std::string& filename);
}