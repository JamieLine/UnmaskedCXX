#ifndef TESTCREATOR_TESTCREATOR_H
#define TESTCREATOR_TESTCREATOR_H

#include <string>

#include "TestCreator/Structs/Filepath.h"

enum TestCreationStatus {
  ALL_OK,
  COULD_NOT_READ_INPUT_FILE,
  COULD_NOT_OPEN_OUTPUT_FILE,
  FOUND_UNEXPECTED_TOKEN_FROM_MAP,
};

auto CreateTestsFromFile(Filepath Source) -> TestCreationStatus;

#endif /* TESTCREATOR_TESTCREATOR_H */
