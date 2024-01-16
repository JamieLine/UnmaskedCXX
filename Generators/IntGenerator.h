/*#pragma once
#include "Generator.h"
#include <random>

template<>
class Generator<int> {
public:
    static int GenerateValue() {
        std::random_device Device;
        std::mt19937 RNG(Device);
        // TODO: Make the bounds parameters
        std::uniform_int_distribution<std::mt19937::result_type> Distribution(-1000, 1000);

        return Distribution(RNG);
    }
};*/

#pragma once
#include <random>

class Generator_int {
public:
    int GenerateValue() {
        std::random_device Device;
        std::mt19937 RNG(Device());
        // TODO: Make the bounds parameters
        std::uniform_int_distribution<std::mt19937::result_type> Distribution(-1000, 1000);

        return Distribution(RNG);
    }
};