#ifndef TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H
#define TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H

#include <string>
#include <vector>

#include "../Consts/TestDefaults.h"
#include "../Structs/Filepath.h"
#include "../Structs/GeneratorParameterStoreSeed.h"
#include "TestCreator/Structs/Filepath.h"

auto CreateAlwaysReturnValueTest(std::vector<std::string>::iterator& FirstToken,
                                 GeneratorParameterStoreSeed Params,
                                 Filepath& TestDefinitionPath,
                                 const std::string& GeneratedFunctionName,
                                 std::size_t NumTestsToRun) -> std::string;

#endif /* TESTCREATOR_PARSERS_CREATEALWAYSRETURNVALUETEST_H */
