#pragma once

#include <string>
#include "../../simulation/Simulation/Simulation.hpp"

// Binary file serialization for the simulation state.
// Format: city name, dimensions, dates, building matrix with residents, city history.
// All strings are length-prefixed (int + raw chars). Files are stored in data/<name>.dat.
namespace Serializer {
    void save(const Simulation& simulation, const std::string& filename);
    void load(Simulation& simulation, const std::string& filename);
}