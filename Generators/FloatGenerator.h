#ifndef GENERATORS_FLOATGENERATOR_H
#define GENERATORS_FLOATGENERATOR_H

#include <iostream>
#include <random>

#include "GeneratorParameterStore.h"
#include "Logging.h"
#include "Optional.h"

class Generator_float {
 public:
  auto GenerateValue(GeneratorParameterStore& Parameters, bool IsUsed)
      -> float {
    if (!IsUsed) {
      Log(std::cout, LOG,
          "Float Generator was told its value will be ignored.");
      std::cout << std::endl;
      return 1;
    }

    std::random_device Device;
    std::mt19937 RNG(Device());

    Optional<float> FetchedLowerBound =
        Parameters.GetFloatParameter(FLOAT_LOWER_BOUND);
    Optional<float> FetchedUpperBound =
        Parameters.GetFloatParameter(FLOAT_UPPER_BOUND);

    float LowerBound;
    float UpperBound;

    if (FetchedLowerBound.DataExists) {
      LowerBound = FetchedLowerBound.Data;
    }
    // Choose a sensible default in the absence of a parameter.
    else {
      LowerBound = -1000.0f;
    }

    if (FetchedUpperBound.DataExists) {
      UpperBound = FetchedUpperBound.Data;
    } else {
      UpperBound = 1000.0f;
    }

    Log(std::cout, LOG, "Float generator invoked.");
    Log(std::cout, LOG, "Upper and lower bounds are");
    Log(std::cout, VALUE_OUTPUT, std::to_string(UpperBound));
    Log(std::cout, VALUE_OUTPUT, std::to_string(LowerBound));
    std::uniform_real_distribution<float> Distribution(LowerBound, UpperBound);

    float ToReturn = Distribution(RNG);
    Log(std::cout, LOG, "Generated value was");
    Log(std::cout, VALUE_OUTPUT, std::to_string(ToReturn));

    std::cout << std::endl;

    return Distribution(RNG);
  }
};

#endif /* GENERATORS_FLOATGENERATOR_H */
