#pragma once

#include <string>


// A POD struct which groups together data regarding a test which has already been completed.
// Used in TestRunnerTemplate and generated runners.
struct TestResult {
public:
	std::string Name;
	std::string Type;

	bool Passed;

	TestResult(std::string InName, std::string InType, bool InPassed) :
		Name(InName), Type(InType), Passed(InPassed) {}
};