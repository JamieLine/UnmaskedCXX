#include <iostream>
#include "TestAuthors/CreateStabilisingSetTest.h"
#include "TestAuthors/CreateAlwaysReturnValueTest.h"
#include "StringOperations.h"
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include "Logging.h"
#include <cerrno>
#include <sys/stat.h>
#include <cstring>
#include "TestSpecification.h"

int main(int argc, char** argv) {
    Log(std::cout, LOG, "UnmaskedCXX started");

    // TODO: MAKE THIS GENERAL
    std::ifstream SourceFileStream;
    SourceFileStream.open("./SampleCXXProject/Calculations.h");

    if (SourceFileStream.is_open()) {
      Log(std::cout, LOG, "Opened file successfully.");
    }

    else {
      Log(std::cout, ERROR, "Error in opening file.");
      return 0;
    }

    std::string CalculationsSourceString( (std::istreambuf_iterator<char>(SourceFileStream) ),
                       (std::istreambuf_iterator<char>()    ) );

    Log(std::cout, LOG, "Created source string.");

    std::vector<std::string> KeptDelimiters = {"&", ",", ";", "(", ")", "{", "}", "[", "]", "<", ">"};
    std::vector<std::string> DiscardedDelimiters = {" ", "\n", "\t", };

    Log(std::cout, LOG, "Started to create test.");

    std::vector<std::string> Tokens = Tokenize(CalculationsSourceString, KeptDelimiters, DiscardedDelimiters);
    // We now have Tokens over the entire file, we want to find the Tokens which are relevant
    // for creating tests.

    auto StartOfTestSpec = std::find(Tokens.begin(), Tokens.end(), "UnmaskedTestStabilisingSet");
    auto EndOfTestSpec = std::find(StartOfTestSpec, Tokens.end(), ";");

    Tokens = std::vector<std::string>(StartOfTestSpec, EndOfTestSpec);

    Log(std::cout, LOG, "Here are the results");

	// This really wants to become a specific "store" of Tests
	// That is more efficient than this
    std::vector<TestSpecification> StabilisingTests = CreateAllStabilisingTests(Tokenize(CalculationsSourceString, KeptDelimiters, DiscardedDelimiters), "./SampleCXXProject/Calculations.h");
	std::vector<TestSpecification> ReturnValueTests = CreateAllAlwaysReturnValueTests(Tokenize(CalculationsSourceString, KeptDelimiters, DiscardedDelimiters), "./SampleCXXProject/Calculations.h");

	std::vector<TestSpecification> Tests;
	Tests.reserve(StabilisingTests.size() + ReturnValueTests.size());
	Tests.insert(Tests.end(), StabilisingTests.begin(), StabilisingTests.end());
	Tests.insert(Tests.end(), ReturnValueTests.begin(), ReturnValueTests.end());

    Log(std::cout, LOG, "Writing results to appropriate files");

	// Generate the source files for each individual test.
	for (auto const& Test : Tests) {
		std::string OutputFileName = Test.Name;

		// Remove any ../ or ./ occurrences from the file name, and then add the file extension.
		const std::vector<std::string> PartsToRemove = {"../", "./",};
		for (auto const& Extension : PartsToRemove) {
			std::string::size_type Index = OutputFileName.find(Extension);

			if (Index != std::string::npos) {
				OutputFileName.erase(Index, Extension.length()); 
			}
		}
		
		OutputFileName.append(".cpp");

		OutputFileName.insert(0, "./UnmaskedCreatedTests/");

		std::ofstream CurrentFile;
		CurrentFile.open(OutputFileName, 
		std::fstream::in |
		std::fstream::out |
		std::fstream::trunc);

		if (!CurrentFile.is_open()) {
			Log(std::cout, VALUE_OUTPUT, std::strerror(errno));
			Log(std::cout, VALUE_OUTPUT, std::to_string(CurrentFile.rdstate() == std::ios_base::failbit));
			Log(std::cout, ERROR, "Attempted to open file and didn't work?");
			Log(std::cout, VALUE_OUTPUT, OutputFileName);
			Log(std::cout, LOG, "To fix the error manually, here are the contents that would have been written");
			Log(std::cout, VALUE_OUTPUT, Test.SourceCode);
		}

		else {
			Log(std::cout, LOG, "Successfully opened file");
			Log(std::cout, VALUE_OUTPUT, OutputFileName);

			CurrentFile << Test.SourceCode;
		}

		CurrentFile.close();
    }

	// Now generate the appropriate "main" file.

	std::ifstream FileRunnerTemplate;
	FileRunnerTemplate.open("./Templates/TestRunnerTemplate.cpp");

	if (FileRunnerTemplate.is_open()) {
      Log(std::cout, LOG, "Opened file successfully.");
    }

    else {
      Log(std::cout, ERROR, "Error in opening FileRunnerTemplate.cpp.");
      return 0;
    }

    std::string TestRunnerSource( (std::istreambuf_iterator<char>(FileRunnerTemplate) ),
                       (std::istreambuf_iterator<char>()    ) );

    Log(std::cout, LOG, "Created TestRunnerSource string from template.");

	// We need to include every generated test in this file.
	// It's not standard to include a .cpp file
	// but this skips awkwardness with CMake.
	std::vector<std::string> GeneratedTestIncludeDirectives;
	GeneratedTestIncludeDirectives.reserve(Tests.size());
	for (auto const& Test : Tests) {
		GeneratedTestIncludeDirectives.push_back("#include \"" + Test.Name + ".cpp\"");
	}

	TestRunnerSource = ReplaceAllInString(TestRunnerSource, "INCLUDE_TESTS", JoinVectorOfStrings(GeneratedTestIncludeDirectives, "\n"));

	// We now need to run those tests and push their results into the Results array in "TestRunnerTemplate.cpp"
	std::vector<std::string> RunTestSource;
	RunTestSource.reserve(Tests.size());

	for (auto const& Test : Tests) {
		// The first call to Test.Name inserts the name into the struct in the new code
		// The second is used to generate a function call
		// Test.Name as a string is a usable, readable name
		// {Test.Name}() calls that function.
		RunTestSource.push_back("\tTests.emplace_back(\"" + Test.Name + "\", \"" + Test.Type + "\", " + Test.Name + "());");
	}

	TestRunnerSource = ReplaceAllInString(TestRunnerSource, "RUN_TESTS_AND_PUSH_RESULTS", JoinVectorOfStrings(RunTestSource, "\n"));

	Log(std::cout, LOG, "Finished TestRunnerSource");

	std::string FileRunnerAddress = "./UnmaskedCreatedTests/TestRunner.cpp";
	std::ofstream TestRunnerFile;
	TestRunnerFile.open(FileRunnerAddress,
						std::fstream::in |
						std::fstream::out |
						std::fstream::trunc);

	if (TestRunnerFile.is_open()) {
		Log(std::cout, LOG, "Successfully opened TestRunner.cpp");
		TestRunnerFile << TestRunnerSource;
		TestRunnerFile.close();
	}

	else {
		Log(std::cout, ERROR, "Failed to open TestRunner.cpp. To recovery manually, here is the data that would be written into that file.");
		Log(std::cout, VALUE_OUTPUT, TestRunnerSource);
	}
}