#pragma once

#include <string>

// TODO: Make the test runner generate these, and make it output these structures to Report.csv

struct TestResult {
public:
	std::string Name;
	std::string Type;

	bool Passed;

	TestResult(std::string InName, std::string InType, bool InPassed) :
		Name(InName), Type(InType), Passed(InPassed) {}
};