#pragma once

#include <string>
#include <vector>

#include "TestSpecification.h"

#include "TestDefaults.h"

const std::string ALWAYS_RETURN_VALUE_TEST_MARKER = "UnmaskedAlwaysReturnsValueTest";

// These batch versions assume the SourceFileAddress and NumTestsToRun are constant across the batch.
// These are also ready to take in an entire file, they will find the tests within the noise.
// The file may also contain other types of tests.
std::vector<TestSpecification> CreateAllAlwaysReturnValueTests(std::vector<std::string> Tokens, std::string SourceFileAddress);
std::vector<TestSpecification> CreateAllAlwaysReturnValueTests(std::vector<std::string> Tokens, std::string SourceFileAddress, std::size_t NumTestsToRun);
