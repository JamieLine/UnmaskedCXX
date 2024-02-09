#ifndef SAMPLECXXPROJECT_CALCULATIONS_H
#define SAMPLECXXPROJECT_CALCULATIONS_H

#include <functional>

#include "../UserlandIncludes/UnmaskedTests.h"

int AddInts(int A, int B) { return A + B; }

int LinearCombination(int A, int X, int B, int Y) { return A * X + B * Y; }

int LinearCombinationWrong(int A, int X, int B, int Y) { return A + X + B * Y; }

void TestSpecifications() {
  UnmaskedSetParameter(INT_LOWER_BOUND, 10);
  UnmaskedSetTempParameter(INT_UPPER_BOUND, 100);

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
}

#endif /* SAMPLECXXPROJECT_CALCULATIONS_H */
