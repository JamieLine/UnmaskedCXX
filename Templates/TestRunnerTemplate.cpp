INCLUDE_TESTS
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "../TestAuthors/TestResult.h"
  
const std::string REPORT_FILE_NAME = "UnmaskedReport.csv";
/*
struct NameResultPair {
public:
	std::string Name;
	bool Passed;

	NameResultPair(std::string InName, bool InPassed) : Name(InName), Passed(InPassed) {}
};*/

std::string BoolToString(bool B) {
	if (B) { return "True"; }
	return "False";
}

int main(int argc, char** argv) {
	//std::vector<NameResultPair> Results;
  	std::vector<TestResult> Tests;

	// Intentionally de-indented for easier generation.
RUN_TESTS_AND_PUSH_RESULTS

	int NumberOfPassedTests = 0;
	for (auto Test : Tests) {
		std::cout << "Test name : " + Test.Name + "\n";
    std::cout << "Test type : " + Test.Type + "\n";
		std::cout << "Test passed? :" + BoolToString(Test.Passed) + "\n";
		if (Test.Passed) {
			NumberOfPassedTests++;
		}
	}

	std::cout << "Summary: " + std::to_string(NumberOfPassedTests) + "/" + std::to_string(Tests.size()) + " tests passed.\n";

	std::cout << "Writing results to a report file.\n";
	std::cout << "Writing to " + REPORT_FILE_NAME;

	std::ofstream ReportFile;
	ReportFile.open("Report.csv",
					std::fstream::in |
					std::fstream::out |
					std::fstream::trunc);

	if (!ReportFile.is_open()) {
		std::cout << "Error, could not open Report.csv\n";
		std::cout << "Report contents are given above.\n";
		std::cout << "Exiting.\n";
	}

	else {
		for (auto const& Test : Tests) {
			ReportFile << Test.Name + "," + Test.Type + "," + BoolToString(Test.Passed) + "\n";
		}

		ReportFile.close();
	}


	// Forcing a buffer flush here.
	std::cout << std::endl;
}
