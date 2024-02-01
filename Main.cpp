#include "TestCreator/TestCreator.h"
#include <iostream>

int main(int argc, char** argv) {
    TestCreationStatus T = CreateTestsFromFile("./SampleCXXProject/Calculations.h");
    std::cout << T << std::endl;
}