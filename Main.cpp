#include <iostream>

#include "Logging.h"
#include "TestCreator/Driver/Driver.h"
#include "TestCreator/TestCreator.h"

auto main(int /*argc*/, char** /*argv*/) -> int {
  Filepath TempTestPath("./SampleCXXProject/Calculations.h");
  // TestCreationStatus TempTest = CreateTestsFromFile(TempTestPath);
  // std::cout << TempTest << std::endl;
  //
  //  TODO: MAKE THIS ACTUALLY FUNCTION CORRECTLY

  Log(std::cout, LOG, "Starting UnmaskedCXX");
  Driver::ParseInputFile(Filepath("./SampleCXXProject/Calculations.h"));

  Driver::WriteAllStoredInputs();
}
