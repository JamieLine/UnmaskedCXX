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
            std::string TestSource = CreateAlwaysReturnValueTest(NextUsefulToken,
                                                              CurrentGeneratorParameters,
                                                              FilePath,
                                                              FilepathToLegalIdentifier(FilePath),
                                                              DEFAULT_NUM_TESTS_TO_RUN);
            
            std::string OutputFilepath = "./UnmaskedCreatedTests/" + FilepathToLegalIdentifier(FilePath) + "_" + ALWAYS_RETURN_VALUE_TEST_MARKER + "_" + std::to_string(CurrentTestNumber) + ".cpp";
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
            std::string TestSource = CreateStabilisingSetTest(NextUsefulToken,
                                                              CurrentGeneratorParameters,
                                                              FilePath,
                                                              FilepathToLegalIdentifier(FilePath),
                                                              DEFAULT_NUM_TESTS_TO_RUN);
            
            std::string OutputFilepath = "./UnmaskedCreatedTests/" + FilepathToLegalIdentifier(FilePath) + "_" + STABILISING_TEST_MARKER + "_" + std::to_string(CurrentTestNumber) + ".cpp";
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

    return CurrentStatus;
}

