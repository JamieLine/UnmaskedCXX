#ifndef GENERATORS_INTGENERATOR_H
#define GENERATORS_INTGENERATOR_H

#include <iostream>
#include <random>

#include "GeneratorParameterStore.h"
#include "Logging.h"
#include "Optional.h"

class Generator_int {
 public:
  auto GenerateValue(GeneratorParameterStore& Parameters, bool IsUsed) -> int {
    if (!IsUsed) {
      Log(std::cout, LOG, "Int Generator was told its value will be ignored.");
      std::cout << std::endl;
      return 1;
    }

    std::random_device Device;
    std::mt19937 RNG(Device());

    Optional<int> FetchedLowerBound =
        Parameters.GetIntegerParameter(INT_LOWER_BOUND);
    Optional<int> FetchedUpperBound =
        Parameters.GetIntegerParameter(INT_UPPER_BOUND);

    int LowerBound;
    int UpperBound;

    if (FetchedLowerBound.DataExists) {
      LowerBound = FetchedLowerBound.Data;
    }
    // Choose a sensible default in the absence of a parameter.
    else {
      LowerBound = -1000;
    }

    if (FetchedUpperBound.DataExists) {
      UpperBound = FetchedUpperBound.Data;
    } else {
      UpperBound = 1000;
    }

    Log(std::cout, LOG, "Int generator invoked.");
    Log(std::cout, LOG, "Upper and lower bounds are");
    Log(std::cout, VALUE_OUTPUT, std::to_string(UpperBound));
    Log(std::cout, VALUE_OUTPUT, std::to_string(LowerBound));
    std::uniform_int_distribution<std::mt19937::result_type> Distribution(
        LowerBound, UpperBound);

    int ToReturn = Distribution(RNG);
    Log(std::cout, LOG, "Generated value was");
    Log(std::cout, VALUE_OUTPUT, std::to_string(ToReturn));

    std::cout << std::endl;

    return Distribution(RNG);
  }
};

#endif /* GENERATORS_INTGENERATOR_H */
