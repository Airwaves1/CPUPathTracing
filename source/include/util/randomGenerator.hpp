#pragma once

#include <cstddef>
#include <random>

class RandomGenerator {

public:
    RandomGenerator() : gen(std::random_device{}()) {}

    RandomGenerator(size_t seed) : gen(seed) {}

    void SetSeed(size_t seed) {
        gen.seed(seed);
    }

    float Uniform(float a = 0.0f, float b = 1.0f) {
        std::uniform_real_distribution<float> uniform_distribution(a, b);
        return uniform_distribution(gen);
    }

private:
    std::mt19937 gen;
};