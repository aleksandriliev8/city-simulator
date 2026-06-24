#include "src/simulation/Simulation/Simulation.hpp"
#include "src/simulation/CommandHandler/CommandHandler.hpp"
#include "src/utils/Random/Random.hpp"

int main() {
    Random::seed();

    Simulation simulation;
    CommandHandler handler(simulation);
    handler.run();

    return 0;
}