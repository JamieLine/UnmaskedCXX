#ifndef SAMPLECXXPROJECT_CALCULATIONS_H
#define SAMPLECXXPROJECT_CALCULATIONS_H

#include <functional>

#include "../UserlandIncludes/UnmaskedTests.h"
#include "VectorCalculations.h"

int AddInts(int A, int B) { return A + B; }

int LinearCombination(int A, int X, int B, int Y) { return A * X + B * Y; }

int LinearCombinationWrong(int A, int X, int B, int Y) { return A + X + B * Y; }

float AddFloats(float A, float B) { return A + B; }

void TestSpecifications() {
  UnmaskedSetParameter(INT_LOWER_BOUND, 10);
  UnmaskedSetParameter(INT_UPPER_BOUND, 100);

  // TODO: Make this include a real file for testing.
  // UnmaskedIncludeFile("SampleCXXProject/Calculations.cpp");

  UnmaskedStabilisingSetTest(
      std::function<int(int, int)>(&AddInts),
      {(GeneratorSettings){.Index = 1, .Fixed = true, .Value = "1"}});
  UnmaskedAlwaysReturnsValueTest(
      std::function<int(int, int)>(&AddInts), 0,
      {(GeneratorSettings){.Index = 0, .Fixed = true, .Value = "2"}});
  UnmaskedAlwaysReturnsValueTest(
      std::function<int(int, int)>(&AddInts), 0,
      {(GeneratorSettings){.Index = 0, .Fixed = true, .Value = "3"},
       (GeneratorSettings){.Index = 1, .Fixed = true, .Value = "4"}});

  UnmaskedSetCategory("Category A");
  UnmaskedSetSeverity(LIGHT);

  UnmaskedPredicateTest(
      std::function<int(int, int, int, int)>(&LinearCombination),
      [](int ReturnedValue) { return ReturnedValue < 10; }, {});

  UnmaskedSetCategory("Category B");
  UnmaskedSetSeverity(SERIOUS);

  UnmaskedPredicateTest(
      std::function<int(int, int, int, int)>(&LinearCombination),
      [](int ReturnedValue) { return ReturnedValue != 0; },
      {(GeneratorSettings){.Index = 1, .Fixed = true, .Value = "5"},
       (GeneratorSettings){
           .Index = 2,
           .GeneratorScript = "[]() { return -GeneratedValues[0]; }"},
       (GeneratorSettings){.Index = 3, .GeneratorType = "Generator_int_2"}});
}

#endif /* SAMPLECXXPROJECT_CALCULATIONS_H */
