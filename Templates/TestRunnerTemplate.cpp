INCLUDE_TESTS
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
RUN_TESTS_AND_PUSH_RESULTS

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