#pragma once

class Random {
public:
    // we don't need any instance of this class
    Random() = delete;

    static void seed();
    static int randomInt(int min, int max);
    static double randomDouble(double min, double max);
    static bool randomBool(double probability = 0.5);
};