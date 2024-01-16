#include <iostream>
#include "TestAuthors/CreateStabilisingSetTest.h"
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

int main(int argc, char** argv) {
    // TODO: This should write into `RunAllTests.cpp`
    // such that that script forms a program which performs all tests

    // TODO: MAKE THIS GENERAL

    Log(std::cout, LOG, "UnmaskedCXX started");

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

    //std::cout << "-------" << std::endl;
    //std::cout << "Printing tokens" << std::endl;

    //for (auto const& Token : Tokenize(CalculationsSourceString, KeptDelimiters, DiscardedDelimiters)) {
    //  std::cout << Token << std::endl;
    //}

    //std::cout << "-------" << std::endl;

    Log(std::cout, LOG, "Here are the results");

    std::vector<NameTestPair> Results = CreateAllStabilisingTests(Tokenize(CalculationsSourceString, KeptDelimiters, DiscardedDelimiters), "./SampleCXXProject/Calculations.h");
    //for (auto const& Result : Results) { 
    //  std::cout << Result.Name;
    //  std::cout << Result.Test;
    //  std::cout << "\n-----------\n";
    //  std::cout << std::endl;
    //}
    //std::cout << std::endl;

    Log(std::cout, LOG, "Writing results to appropriate files");

	for (auto const& Result : Results) {
		std::string OutputFileName = Result.Name;

		// Remove the extension from the file name, and then re-add it.
		// Also remove any ../ or ./ occurrences.
		const std::vector<std::string> PartsToRemove = {"../", "./", ".h", ".hpp", ".cxx", ".c", ".cpp", ".hxx"};
		for (auto const& Extension : PartsToRemove) {
			std::string::size_type Index = OutputFileName.find(Extension);

			if (Index != std::string::npos)
   				OutputFileName.erase(Index, Extension.length());
			//OutputFileName.erase(std::remove(OutputFileName.begin(), OutputFileName.end(), Extension), OutputFileName.end());
		}

		// Replace `/` in the directory tree with `_` to avoid Directory Trickery
		OutputFileName = ReplaceAllInString(OutputFileName, "/", "_");
		OutputFileName = ReplaceAllInString(OutputFileName, "\\", "_");
		
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
			Log(std::cout, VALUE_OUTPUT, Result.Test);
		}

		else {
			Log(std::cout, LOG, "Successfully opened file");
			Log(std::cout, VALUE_OUTPUT, OutputFileName);

			CurrentFile << Result.Test;
		}

		CurrentFile.close();
    }
}