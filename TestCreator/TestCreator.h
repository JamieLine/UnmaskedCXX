#ifndef TESTCREATOR_TESTCREATOR_H
#define TESTCREATOR_TESTCREATOR_H

#include <string>

#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/TestCreationStatus.h"

auto CreateTestsFromFile(Filepath Source) -> TestCreationStatus;

#endif /* TESTCREATOR_TESTCREATOR_H */
