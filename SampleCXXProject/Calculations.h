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
  UnmaskedSetTempParameter(INT_UPPER_BOUND, 100);

  UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0);

  UnmaskedIncludeFile("SampleCXXProject/VectorCalculations.h");
  UnmaskedSetCategory("Test Category A");

  UnmaskedStabilisingSetTest(std::function<int(int, int)>(&AddInts), 0, 0);

  UnmaskedSetTempParameter(INT_LOWER_BOUND, 990);
  UnmaskedStabilisingSetTest(
      std::function<int(int, int, int, int)>(&LinearCombination), 0, 0, 2, 0);
  UnmaskedStabilisingSetTest(
      std::function<int(int, int, int, int)>(&LinearCombinationWrong), 0, 0, 2,
      0);
  UnmaskedStabilisingSetTest(
      std::function<int(int, int, int, int)>(&LinearCombination), 1, 0, 3, 0);

  UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0, 0,
                                 0, 1, 0);
  UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 5, 0,
                                 0, 1, 0);

  UnmaskedSetCategory("Test Category B");

  UnmaskedStabilisingSetTest(std::function<float(float, float)>(&AddFloats), 0,
                             0.0f, 1, 0.0f);
  UnmaskedAlwaysReturnsValueTest(std::function<float(float, float)>(&AddFloats),
                                 5.0f, 0, 0.0f, 1, 5.0f);
  UnmaskedAlwaysReturnsValueTest(std::function<float(float, float)>(&AddFloats),
                                 5.0f, 0, 0.0f, 1, 2.0f);
  UnmaskedStabilisingSetTest(std::function<float(float, float)>(&AddFloats), 0,
                             0.1f);

  UnmaskedSetParameter(FLOAT_UPPER_BOUND, 10.0f);

  UnmaskedSetTempParameter(FLOAT_LOWER_BOUND, -1.0f);
  UnmaskedStabilisingSetTest(std::function<float(float, float)>(&AddFloats), 0,
                             0.2f);

  UnmaskedStabilisingSetTest(std::function<float(float, float)>(&AddFloats), 1,
                             100.0f);

  UnmaskedAlwaysReturnsValueTest(
      std::function<float(float, float, float, float)>(&Vec2DotProduct), 0, 0,
      0.0f);
}

#endif /* SAMPLECXXPROJECT_CALCULATIONS_H */
