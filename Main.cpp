#include <iostream>

#include "TestCreator/TestCreator.h"

auto main(int /*argc*/, char** /*argv*/) -> int {
  Filepath TempTestPath("./SampleCXXProject/Calculations.h");
  TestCreationStatus TempTest = CreateTestsFromFile(TempTestPath);
  std::cout << TempTest << std::endl;
}