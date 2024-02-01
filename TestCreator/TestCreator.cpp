#include "TestCreator.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../StringOperations.h"
#include "../Optional.h"
#include "Consts/TestMarkers.h"
#include "../Logging.h"
#include <iostream>
#include "Structs/GeneratorParameterStoreSeed.h"
#include "Parsers/CreateStabilisingSetTest.h"
#include "Parsers/CreateAlwaysReturnValueTest.h"
#include "Structs/TestSpecification.h"

void FindNextOccurrences(
    std::map<std::string, std::vector<std::string>::iterator>& Map,
    std::vector<std::string>& Tokens,
    std::vector<std::string>::iterator& Start) 
{
    Map[STABILISING_TEST_MARKER] = std::find(Start, Tokens.end(), STABILISING_TEST_MARKER);
    Map[ALWAYS_RETURN_VALUE_TEST_MARKER] = std::find(Start, Tokens.end(), ALWAYS_RETURN_VALUE_TEST_MARKER);
    Map[SET_PARAMETER_MARKER] = std::find(Start, Tokens.end(), SET_PARAMETER_MARKER);
    Map[SET_TEMP_PARAMETER_MARKER] = std::find(Start, Tokens.end(), SET_TEMP_PARAMETER_MARKER);
}

TestCreationStatus CreateTestsFromFile(std::string FilePath) {
    // Begin by setting some niceties for this function.
    using std::string;
    using std::vector;

    using TokensIterator = vector<string>::iterator;
    
    TestCreationStatus CurrentStatus = ALL_OK;

    vector<TestSpecification> GeneratedTestSpecs;
    

    // Check that this filepath is good.
    Optional<string> MaybeFileSource = ReadContentsOfFile(FilePath);

    if (!MaybeFileSource.DataExists) {
        return COULD_NOT_READ_INPUT_FILE;
    }

    // We wish to tokenize this file but some delimiters are important and some can be discarded.
	// Broadly, it is important if it carries specific meaning.
    const vector<string> KeptDelimiters = {"&", ",", ";", "(", ")", "{", "}", "[", "]", "<", ">"};
    const vector<string> DiscardedDelimiters = {" ", "\n", "\t", };

    vector<string> Tokens = Tokenize(MaybeFileSource.Data, KeptDelimiters, DiscardedDelimiters);
    TokensIterator NextUsefulToken = Tokens.begin();    

    // This map will be used to find the nearest marker of interest
    std::map<string, TokensIterator> FoundMarkers;

    int CurrentTestNumber = 0;

    GeneratorParameterStoreSeed CurrentGeneratorParameters;

    while (NextUsefulToken != Tokens.end()) {
        FindNextOccurrences(FoundMarkers, Tokens, NextUsefulToken);

        // We're going to get an iterator into the map which tells us the
        // (Marker, TokensIterator) pair which minimises the Iterator while 
        // still moving us forward in the file.

        // Then dereference that iterator into a map, and fetch the TokensIterator
        auto MapIterator = std::min_element(std::begin(FoundMarkers), std::end(FoundMarkers),
                           [](const auto& l, const auto& r) { return l.second < r.second; });

        NextUsefulToken = (*MapIterator).second;
        Log(std::cout, LOG, "Found NextUsefulToken");

        // The next segment can't be a switch statment because C++ won't let you // TODO: IMPLEMENT LOGIhave a switch on strings
        // You can technically do it if you hash the strings to an integer
        // But here that's more effort than it's worth.
        // The if's aren't less readable they're just less idiomatic// TODO: IMPLEMENT LOGI
        // and this comment tells you that spiritually this is a switch.

        if (NextUsefulToken == Tokens.end()) {
            Log(std::cout, LOG, "Found final token in TestCreator");
            break;
        }
        else if (*NextUsefulToken == ALWAYS_RETURN_VALUE_TEST_MARKER) {
            Log(std::cout, LOG, "Found ALWAYS_RETURN_VALUE_TEST_MARKER in TestCreator");

            string OutputFilepath = "./UnmaskedCreatedTests/" + FilepathToLegalIdentifier(FilePath) + "_" + ALWAYS_RETURN_VALUE_TEST_MARKER + "_" + std::to_string(CurrentTestNumber) + ".cpp";

            string TestSource = CreateAlwaysReturnValueTest(NextUsefulToken,
                                                              CurrentGeneratorParameters,
                                                              FilePath,
                                                              FilepathToLegalIdentifier(OutputFilepath),
                                                              DEFAULT_NUM_TESTS_TO_RUN);
            
            
            GeneratedTestSpecs.emplace_back(OutputFilepath, ALWAYS_RETURN_VALUE_TEST_MARKER);
            CurrentTestNumber++;
            bool FileOutputWasSuccess = WriteStringIntoFileOverwriting(OutputFilepath, TestSource);

            if (!FileOutputWasSuccess) {
                Log(std::cout, LOG, "Error writing " + ALWAYS_RETURN_VALUE_TEST_MARKER + " to file. Dumping filepath and test source.");
                Log(std::cout, VALUE_OUTPUT, OutputFilepath);
                Log(std::cout, VALUE_OUTPUT, TestSource);

                CurrentStatus = COULD_NOT_OPEN_OUTPUT_FILE;
            }
            
            CurrentGeneratorParameters.ResetTempParameters();
        }
        else if (*NextUsefulToken == STABILISING_TEST_MARKER) {
            Log(std::cout, LOG, "Found STABILISING_TEST_MARKER in TestCreator");

            std::string OutputFilepath = "./UnmaskedCreatedTests/" + FilepathToLegalIdentifier(FilePath) + "_" + STABILISING_TEST_MARKER + "_" + std::to_string(CurrentTestNumber) + ".cpp";

            std::string TestSource = CreateStabilisingSetTest(NextUsefulToken,
                                                              CurrentGeneratorParameters,
                                                              FilePath,
                                                              FilepathToLegalIdentifier(OutputFilepath),
                                                              DEFAULT_NUM_TESTS_TO_RUN);
            
            
            std::string CurrentTestName = FilepathToLegalIdentifier(FilePath) + "_" + STABILISING_TEST_MARKER + "_" + std::to_string(CurrentTestNumber);
            GeneratedTestSpecs.emplace_back(OutputFilepath, STABILISING_TEST_MARKER);
            CurrentTestNumber++;
            bool FileOutputWasSuccess = WriteStringIntoFileOverwriting(OutputFilepath, TestSource);

            if (!FileOutputWasSuccess) {
                Log(std::cout, LOG, "Error writing " + ALWAYS_RETURN_VALUE_TEST_MARKER + " to file. Dumping filepath and test source.");
                Log(std::cout, VALUE_OUTPUT, OutputFilepath);
                Log(std::cout, VALUE_OUTPUT, TestSource);

                CurrentStatus = COULD_NOT_OPEN_OUTPUT_FILE;
            }

            CurrentGeneratorParameters.ResetTempParameters();
        }
        else if (*NextUsefulToken == SET_PARAMETER_MARKER) {
            Log(std::cout, LOG, "Found SET_PARAMETER_MARKER in TestCreator");
            CurrentGeneratorParameters.ReadInParameterDeclaration(NextUsefulToken);
        }
        else if (*NextUsefulToken == SET_TEMP_PARAMETER_MARKER) {
            Log(std::cout, LOG, "Found SET_TEMP_PARAMETER_MARKER in TestCreator");
            CurrentGeneratorParameters.ReadInTempParameterDeclaration(NextUsefulToken);
        }
        else {
            CurrentStatus = FOUND_UNEXPECTED_TOKEN_FROM_MAP;
            Log(std::cout, LOG, "NextUsefulToken dereferenced into something not handled by the \"switch statement\"? Continuing as possible. NextUsefulToken deref'd to");
            Log(std::cout, VALUE_OUTPUT, *NextUsefulToken);
        }
    }

    // Now we must generate the TestRunner.cpp file

    Optional<std::string> MaybeTestRunnerTemplate = ReadContentsOfFile("Templates/TestRunnerTemplate.cpp");

    if (!MaybeTestRunnerTemplate.DataExists) {
        Log(std::cout, ERROR, "Could not open TestRunnerTemplate.cpp");
        CurrentStatus = COULD_NOT_READ_INPUT_FILE;
        return CurrentStatus;
    }

    std::string TestRunnerSource = MaybeTestRunnerTemplate.Data;

    Log(std::cout, LOG, "Created TestRunnerSource string from template.");

	// We need to include every generated test in this file.
	// It's not standard to include a .cpp file
	// but this skips awkwardness with CMake.
	std::vector<std::string> GeneratedTestIncludeDirectives;
	GeneratedTestIncludeDirectives.reserve(GeneratedTestSpecs.size());
	for (auto const& Test : GeneratedTestSpecs) {
		GeneratedTestIncludeDirectives.push_back("#include \"" + Test.Filepath + "\"");
	}

	TestRunnerSource = ReplaceAllInString(TestRunnerSource, "INCLUDE_TESTS", JoinVectorOfStrings(GeneratedTestIncludeDirectives, "\n"));

	// We now need to run those tests and push their results into the Results array in "TestRunnerTemplate.cpp"
	std::vector<std::string> RunTestSource;
	RunTestSource.reserve(GeneratedTestSpecs.size());

	for (auto const& Test : GeneratedTestSpecs) {
		// The first call to Test.Name inserts the name into the struct in the new code
		// The second is used to generate a function call
		// Test.Name as a string is a usable, readable name
		// {Test.Name}() calls that function.
		RunTestSource.push_back("\tTests.emplace_back(\"" + FilepathToLegalIdentifier(Test.Filepath) + "\", \"" + Test.Type + "\", " + FilepathToLegalIdentifier(Test.Filepath) + "());");
	}

	TestRunnerSource = ReplaceAllInString(TestRunnerSource, "RUN_TESTS_AND_PUSH_RESULTS", JoinVectorOfStrings(RunTestSource, "\n"));

	Log(std::cout, LOG, "Finished TestRunnerSource");

    bool OutputSuccessful = WriteStringIntoFileOverwriting("./UnmaskedCreatedTests/TestRunner.cpp", TestRunnerSource);
    if (!OutputSuccessful) {
        Log(std::cout, ERROR, "Failed to write to TestRunner.cpp. To recover manually, here is the data that would be written into that file.");
		Log(std::cout, VALUE_OUTPUT, TestRunnerSource);
    }
    
    else {
        Log(std::cout, LOG, "Successfully wrote TestRunner.cpp");
    }

    return CurrentStatus;
}

