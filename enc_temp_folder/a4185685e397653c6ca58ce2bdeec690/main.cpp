// City Simulation — OOP Course Project
// Entry point: initializes RNG, creates the simulation engine, and starts the REPL.

#include "src/simulation/Simulation/Simulation.hpp"
#include "src/io/CommandHandler/CommandHandler.hpp"
#include "src/utils/Random/Random.hpp"

int main() {
    Random::seed();

    Simulation simulation;
    CommandHandler handler(simulation);
    handler.run();

    return 0;
}