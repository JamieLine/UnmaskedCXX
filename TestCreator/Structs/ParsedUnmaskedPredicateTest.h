#ifndef TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST_H
#define TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST_H

#include "TestCreator/Structs/ParsedFunction.h"
struct ParsedUnmaskedPredicateTest {
  ParsedFunction TestedFunction;
  std::string PredicateSource;

  std::string GeneratorSource;
};

#endif /* TESTCREATOR_STRUCTS_PARSEDUNMASKEDPREDICATETEST_H */
