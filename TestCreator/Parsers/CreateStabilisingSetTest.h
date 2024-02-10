#ifndef TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H
#define TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H

#include <string>
#include <vector>

#include "../Consts/TestDefaults.h"
#include "../Structs/GeneratorParameterStoreSeed.h"
#include "TestCreator/Structs/Filepath.h"

auto CreateStabilisingSetTest(
    std::vector<std::string>::iterator& FirstToken,
    GeneratorParameterStoreSeed Params, Filepath& TestDefinitionPath,
    const std::vector<std::string>& AdditionalIncludes,
    const std::string& GeneratedFunctionName, std::size_t NumTestsToRun)
    -> std::string;

#endif /* TESTCREATOR_PARSERS_CREATESTABILISINGSETTEST_H */
