#pragma once

#include "../UserlandIncludes/UnmaskedTests.h"
#include <functional>

int AddInts(int A, int B) {
    return A + B;
}

int LinearCombination(int A, int X, int B, int Y) {
    return A*X + B*Y;
}

int LinearCombinationWrong(int A, int X, int B, int Y) {
    return A+X + B*Y;
}

void TestSpecifications() {
    UnmaskedStabilisingSetTest(std::function<int(int, int)>(&AddInts), 0, 0);
    UnmaskedStabilisingSetTest(std::function<int(int, int, int, int)>(&LinearCombination), 0, 0, 2, 0);
    UnmaskedStabilisingSetTest(std::function<int(int, int, int, int)>(&LinearCombinationWrong), 0, 0, 2, 0);
	UnmaskedStabilisingSetTest(std::function<int(int, int, int, int)>(&LinearCombination), 1, 0, 3, 0);

	UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0, 0, 0, 1, 0);
	UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 5, 0, 0, 1, 0);
}