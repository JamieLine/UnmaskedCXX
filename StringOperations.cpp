#include "StringOperations.h"

#include "Logging.h"

#include <iostream>
#include <numeric>
#include <algorithm>

std::string ReplaceAllInString(std::string Destination, std::string OldContent, std::string NewContent) {
    auto Index = Destination.find(OldContent);
    
    while (Index != std::string::npos) {
        //line.erase( pos, oldSize );
        //line.insert( pos, newString );
        //Destination.replace(Index, sizeof(OldContent) - 1, NewContent);
        Destination.replace(Index, OldContent.size(), NewContent);
        Index = Destination.find(OldContent);
    }

    return Destination;
}

std::string JoinVectorOfStrings(std::vector<std::string> Strings, std::string Delimiter) {
    return std::accumulate(
        std::next(Strings.begin()), 
        Strings.end(), 
        Strings[0], 
        [Delimiter](std::string a, std::string b) {
            return a + Delimiter + b;
        }   
    );
}

std::vector<std::string> Tokenize(std::string ToTokenize, std::vector<std::string> KeptDelimiters, std::vector<std::string> DiscardedDelimiters) {
    // Again, this function is much nicer with a few using std::* statements
    using std::size_t;
    using std::string;
    using std::vector;
    using std::all_of;
    using std::distance;
    using std::min_element;

    size_t StartIndex = 0;
    size_t EndIndex;
    
    string CurrentToken;
    vector<string> ToReturn;

    // We're merging these because the difference between Kept and Discarded is minimal
    vector<string> Delimiters;
    // This could become std::merge?
    // But its expected that the delimiter strings are "small"
    for (auto Delim : KeptDelimiters) { Delimiters.push_back(Delim); }
    for (auto Delim : DiscardedDelimiters) { Delimiters.push_back(Delim); }

    vector<size_t> DelimiterIndexes;
    DelimiterIndexes.reserve(Delimiters.size());

    // Later on, we want to access this by index, so we need a value to be there in advance.
    // Avoids crashes of the form `DelimiterIndexes[4] = 3` when only up to `DelimiterIndexes[2]` is defined
    for (auto _ : Delimiters) {
        DelimiterIndexes.push_back(string::npos);
    }

    // Populate DelimiterIndexes
    for (size_t Index = 0; Index < Delimiters.size(); Index++) {
        DelimiterIndexes[Index] = ToTokenize.find(Delimiters[Index], StartIndex);
    }

    // while at least one delimiter can be found
    while (!all_of(DelimiterIndexes.begin(), DelimiterIndexes.end(),
            [](size_t x) { return x == string::npos; })) {
        // Get an iterator that points to the next delimiter in `ToTokenize`
        auto NextDelimiterIterator = min_element(DelimiterIndexes.begin(), DelimiterIndexes.end());

    // The distance call converts the iterator to a vector index in `DelimiterIndexes`
        // It's important to note here that `DelimiterIndexes` contains string indexes
        // And our current calculation is regarding vector indexes which point to those
        // string indexes.
        string NextDelimiter = Delimiters[distance(DelimiterIndexes.begin(), NextDelimiterIterator)];

        // We already know the next delimiter exists in `ToTokenize`
        // It's implied by the condition to enter this loop

        ///TODO: DELETE THIS
        //if (((*NextDelimiterIterator) - StartIndex) == 0) {
        //    std::cout << "GOT IT" << std::endl;
        //}
        ToReturn.push_back(ToTokenize.substr(StartIndex, (*NextDelimiterIterator) - StartIndex));
        // Some delimiters are kept, some are discarded.
        // Where a delimiter is kept, it becomes its own token in the token stream.

        // This is shorthand to make the next line easier to read.
        // No additional code should be generated here.

        if (NextDelimiter == " ") {
            Log(std::cout, LOG, "A space char has been found.");
        }

        if (NextDelimiter == "") {
            Log(std::cout, LOG, "An empty string has been found.");
        }

        vector<string> const& Kepts = KeptDelimiters;
        if (std::find(Kepts.begin(), Kepts.end(), NextDelimiter) != Kepts.end()) { ToReturn.push_back(NextDelimiter); } // We wish to keep the delimiters in the final result.
        StartIndex = (*NextDelimiterIterator) + NextDelimiter.length();

        if (NextDelimiter == " " || NextDelimiter == "") {
            Log(std::cout, LOG, "The last token in the stream at this point is");
            Log(std::cout, VALUE_OUTPUT, *(ToReturn.end()-1));
        }

        // Update DelimiterIndexes
        for (size_t Index = 0; Index < Delimiters.size(); Index++) {
            DelimiterIndexes[Index] = ToTokenize.find(Delimiters[Index], StartIndex);
        }

    }

    // Empty strings can appear
    // We pull those out here
    ToReturn.erase(
        std::remove_if(
            ToReturn.begin(), ToReturn.end(),
            [](string s){ return s == "";}),
        ToReturn.end());

    Log(std::cout, LOG, "Tokenizer output is");
    for (auto const& Token : ToReturn) {
        Log(std::cout, VALUE_OUTPUT, Token);
        Log(std::cout, VALUE_OUTPUT, "-|-|-|-|-|-");
    }

    return ToReturn;
}