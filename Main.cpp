#include <iostream>

#include "Logging.h"
#include "TestCreator/Driver/Driver.h"
#include "TestCreator/TestCreator.h"

auto main(int /*argc*/, char** /*argv*/) -> int {
  Log(std::cout, LOG, "Starting UnmaskedCXX");
  Driver::ParseInputFile(Filepath("./SampleCXXProject/Calculations.h"));

  Driver::WriteAllStoredInputs();
}
