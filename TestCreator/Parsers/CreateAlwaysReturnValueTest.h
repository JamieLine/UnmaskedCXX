#pragma once

#include <string>
#include <vector>

#include "../Consts/TestDefaults.h"
#include "../Structs/GeneratorParameterStoreSeed.h"

std::string CreateAlwaysReturnValueTest(std::vector<std::string>::iterator& FirstToken, GeneratorParameterStoreSeed Params, std::string SourceFileAddress, std::string OutputFileNameNoExtension, std::size_t NumTestsToRun);

// These batch versions assume the SourceFileAddress and NumTestsToRun are constant across the batch.
// These are also ready to take in an entire file, they will find the tests within the noise.
// The file may also contain other types of tests.
//std::vector<TestSpecification> CreateAllAlwaysReturnValueTests(std::vector<std::string> Tokens, std::string SourceFileAddress);
//std::vector<TestSpecification> CreateAllAlwaysReturnValueTests(std::vector<std::string> Tokens, std::string SourceFileAddress, std::size_t NumTestsToRun);
