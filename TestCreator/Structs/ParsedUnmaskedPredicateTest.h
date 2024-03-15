#ifndef TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST
#define TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST

#include "Optional.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/GeneratorSettingDescriptor.h"
#include "TestCreator/Structs/ParsedFunction.h"
struct ParsedUnmaskedPredicateTest {
  ParsedFunction TestedFunction;
  std::string PredicateSource;

  GeneratorSettingBunch GeneratorSettings;
};

#endif /* TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST */
