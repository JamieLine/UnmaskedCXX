#include "SampleCXXProject_Calculations0.cpp"
#include "SampleCXXProject_Calculations1.cpp"
#include "SampleCXXProject_Calculations2.cpp"
#include "SampleCXXProject_Calculations3.cpp"
#include <vector>
#include <string>
#include <iostream>

struct NameResultPair {
public:
	std::string Name;
	bool Passed;

	NameResultPair(std::string InName, bool InPassed) : Name(InName), Passed(InPassed) {}
};

std::string BoolToString(bool B) {
	if (B) { return "True"; }
	return "False";
}

int main(int argc, char** argv) {
	std::vector<NameResultPair> Results;

	// Intentionally de-indented for easier generation.
	Results.emplace_back("SampleCXXProject_Calculations0", SampleCXXProject_Calculations0());
	Results.emplace_back("SampleCXXProject_Calculations1", SampleCXXProject_Calculations1());
	Results.emplace_back("SampleCXXProject_Calculations2", SampleCXXProject_Calculations2());
	Results.emplace_back("SampleCXXProject_Calculations3", SampleCXXProject_Calculations3());

	int NumberOfPassedTests = 0;
	for (auto Result : Results) {
		std::cout << "Test name : " + Result.Name + "\n";
		std::cout << "Test passed? :" + BoolToString(Result.Passed) + "\n";
		if (Result.Passed) {
			NumberOfPassedTests++;
		}
	}


	// Forcing a buffer flush here.
	std::cout << "Summary: " + std::to_string(NumberOfPassedTests) + "/" + std::to_string(Results.size()) + " tests passed." << std::endl;
}