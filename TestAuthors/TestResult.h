#pragma once

#include <string>

struct TestResult {
public:
	std::string Name;
	std::string Type;

	bool Passed;

	TestResult(std::string InName, std::string InType, bool InPassed) :
		Name(InName), Type(InType), Passed(InPassed) {}
};