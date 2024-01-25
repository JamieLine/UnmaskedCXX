#include "TestCreator.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../StringOperations.h"
#include "../Optional.h"
#include "TestMarkers.h"
#include "../Logging.h"
#include <iostream>

void FindNextOccurrences(
    std::map<std::string, std::vector<std::string>::iterator>& Map,
    std::vector<std::string> Tokens,
    std::vector<std::string>::iterator Start) 
{
    Map[STABILISING_TEST_MARKER] = std::find(Start, Tokens.end(), STABILISING_TEST_MARKER);
    Map[ALWAYS_RETURN_VALUE_TEST_MARKER] = std::find(Start, Tokens.end(), ALWAYS_RETURN_VALUE_TEST_MARKER);
    Map[SET_PARAMETER_MARKER] = std::find(Start, Tokens.end(), SET_PARAMETER_MARKER);
    Map[SET_TEMP_PARAMETER_MARKER] = std::find(Start, Tokens.end(), SET_TEMP_PARAMETER_MARKER);
}

TestCreationStatus CreateTestsFromFile(std::string FilePath) {
    using std::string;
    using std::vector;

    TestCreationStatus CurrentStatus = ALL_OK;

    Optional<string> MaybeFileSource = FilepathToString(FilePath);

    if (!MaybeFileSource.DataExists) {
        return COULD_NOT_READ_INPUT_FILE;
    }

    // We wish to tokenize this file but some delimiters are important and some can be discarded.
	// Broadly, it is important if it carries specific meaning.
    const vector<string> KeptDelimiters = {"&", ",", ";", "(", ")", "{", "}", "[", "]", "<", ">"};
    const vector<string> DiscardedDelimiters = {" ", "\n", "\t", };

    vector<string> Tokens = Tokenize(MaybeFileSource.Data, KeptDelimiters, DiscardedDelimiters);
    auto CurrentToken = Tokens.begin();

    using TokensIterator = vector<string>::iterator;

    std::map<string, TokensIterator> FoundMarkers;

    while (CurrentToken != Tokens.end()) {
        FindNextOccurrences(FoundMarkers, Tokens, CurrentToken);

        // We're going to get an iterator into the map which tells us the
        // (Marker, TokensIterator) pair which minimises the Iterator while 
        // still moving us forward in the file.

        // Then dereference that iterator into a map, and fetch the TokensIterator
        auto MapIterator = std::min_element(std::begin(FoundMarkers), std::end(FoundMarkers),
                           [](const auto& l, const auto& r) { return l.second < r.second; });

        TokensIterator NextUsefulToken = (*MapIterator).second;

        // This can't be a switch statment because C++ won't let you have a switch on strings
        // You can technically do it if you hash the strings to an integer
        // But here that's more effort than it's worth.
        // The if's aren't less readable they're just less idiomatic
        // and this comment tells you that spiritually this is a switch.
        if (*NextUsefulToken == STABILISING_TEST_MARKER) {
            // TODO: IMPLEMENT LOGIC
        }
        else if (*NextUsefulToken == ALWAYS_RETURN_VALUE_TEST_MARKER) {
            // TODO: IMPLEMENT LOGIC
        }
        else if (*NextUsefulToken == SET_PARAMETER_MARKER) {
            // TODO: IMPLEMENT LOGIC
        }
        else if (*NextUsefulToken == SET_TEMP_PARAMETER_MARKER) {
            // TODO: IMPLEMENT LOGIC
        }
        else {
            CurrentStatus = FOUND_UNEXPECTED_TOKEN_FROM_MAP;
            Log(std::cout, LOG, "NextUsefulToken dereferenced into something not handled by the \"switch statement\"? Continuing as possible. NextUsefulToken deref'd to");
            Log(std::cout, VALUE_OUTPUT, *NextUsefulToken);
        }
    }
}