#ifndef TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H
#define TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H

#include <string>
#include <vector>

#include "../../Consts/TestDefaults.h"
#include "../../Structs/Filepath.h"
#include "../../Structs/GeneratorParameterStoreSeed.h"
#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/TestCreationContext.h"

auto CreateAlwaysReturnValueTest(std::vector<std::string>::iterator& FirstToken,
                                 TestCreationContext Context) -> std::string;

#endif /* TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H */
