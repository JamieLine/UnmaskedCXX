#ifndef TESTCREATOR_STRUCTS_GENERATORSETTINGBUNCH
#define TESTCREATOR_STRUCTS_GENERATORSETTINGBUNCH

#include <vector>

#include "TestCreator/Structs/GeneratorSettingDescriptor.h"
struct GeneratorSettingBunch {
 public:
  std::vector<GeneratorSettingDescriptor> Settings;

  // TODO: THIS BOOL NEEDS TO BECOME A PREDICATE
  // bool IsValid;

  explicit GeneratorSettingBunch();
};

#endif /* TESTCREATOR_STRUCTS_GENERATORSETTINGBUNCH */
