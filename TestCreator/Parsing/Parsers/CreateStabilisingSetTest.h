#ifndef TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H
#define TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H

#include <string>
#include <vector>

#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/TestCreationContext.h"

auto CreateStabilisingSetTest(std::vector<std::string>::iterator& FirstToken,
                              TestCreationContext Context) -> std::string;

#endif /* TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H */
