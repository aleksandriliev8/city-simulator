#include "Random.hpp"
#include <cstdlib>
#include <ctime>

// sets the generator starting point when rand() is called
void Random::seed() {
    std::srand((unsigned int)std::time(nullptr));
}

int Random::randomInt(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

double Random::randomDouble(double min, double max) {
    return min + (double)std::rand() / RAND_MAX * (max - min);
}

bool Random::randomBool(double probability) {
    return randomDouble(0.0, 1.0) < probability;
}