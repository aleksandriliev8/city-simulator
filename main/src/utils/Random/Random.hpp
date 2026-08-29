#pragma once

namespace Random {
    void seed();
    int randomInt(int min, int max);
    double randomDouble(double min, double max);
    bool randomBool(double probability = 0.5);
}