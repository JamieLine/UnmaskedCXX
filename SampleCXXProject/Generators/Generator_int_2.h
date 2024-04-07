#ifndef GENERATORS_INTGENERATOR_2_H
#define GENERATORS_INTGENERATOR_2_H

#include <iostream>

#include "Generators/GeneratorParameterStore.h"
#include "Logging.h"

class Generator_int_2 {
 public:
  auto GenerateValue(GeneratorParameterStore& Parameters, bool IsUsed) -> int {
    if (!IsUsed) {
      Log(std::cout, LOG,
          "Int Generator 2 was told its value will be ignored.");
      std::cout << std::endl;
      return 1;
    }

    Log(std::cout, LOG, "Int Generator 2 called. Returning 10.");
    return 10;
  }
};

#endif /* GENERATORS_INTGENERATOR_2_H */
