#include "StringOperations.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>

#include "Logging.h"

auto ReplaceAllInString(std::string Destination, const std::string& OldContent,
                        const std::string& NewContent) -> std::string {
  auto Index = Destination.find(OldContent);

  while (Index != std::string::npos) {
    Destination.replace(Index, OldContent.size(), NewContent);
    Index = Destination.find(OldContent);
  }

  return Destination;
}

auto JoinVectorOfStrings(std::vector<std::string> Strings,
                         const std::string& Delimiter) -> std::string {
  if (Strings.empty()) {
    return "";
  }

  return std::accumulate(std::next(Strings.begin()), Strings.end(), Strings[0],
                         [Delimiter](const std::string& Prefix,
                                     const std::string& Suffix) -> std::string {
                           return Prefix + Delimiter + Suffix;
                         });
}

auto Tokenize(const std::string& ToTokenize,
              std::vector<std::string> KeptDelimiters,
              std::vector<std::string> DiscardedDelimiters)
    -> std::vector<std::string> {
  // Again, this function is much nicer with a few using std::* statements
  using std::all_of;
  using std::distance;
  using std::min_element;
  using std::size_t;
  using std::string;
  using std::vector;

  size_t StartIndex = 0;

  string CurrentToken;
  vector<string> ToReturn;

  // We're merging these because the difference between Kept and Discarded is
  // minimal
  vector<string> Delimiters;
  // This could become std::merge?
  // But its expected that the delimiter strings are "small"
  Delimiters.reserve(KeptDelimiters.size());

  std::copy(KeptDelimiters.begin(), KeptDelimiters.end(),
            std::back_inserter(Delimiters));
  std::copy(DiscardedDelimiters.begin(), DiscardedDelimiters.end(),
            std::back_inserter(Delimiters));

  // DelimiterIndexes contains the index at which each type of delimiter is
  // found in the `ToTokenize" string.

  // It forms a mapping where for example
  // Delimiters = [",", ".", ...]
  // DelimiterIndexes = [1, 10, ...]
  // And therefore "," gets related to 1 and "." gets related to 10

  // This could become a map later, but it is nice to avoid various
  // Hashing calculations in the process.
  vector<size_t> DelimiterIndexes;
  DelimiterIndexes.reserve(Delimiters.size());

  // Later on, we want to access this by index, so we need a value to be there
  // in advance. Avoids crashes of the form `DelimiterIndexes[4] = 3` when only
  // up to `DelimiterIndexes[2]` is defined
  for (auto Delim : Delimiters) {
    DelimiterIndexes.push_back(string::npos);
  }

  // Populate DelimiterIndexes
  for (size_t Index = 0; Index < Delimiters.size(); Index++) {
    DelimiterIndexes[Index] = ToTokenize.find(Delimiters[Index], StartIndex);
  }

  // while at least one delimiter can be found
  while (!all_of(DelimiterIndexes.begin(), DelimiterIndexes.end(),
                 [](size_t FirstInstance) -> bool {
                   return FirstInstance == string::npos;
                 })) {
    // Get an iterator that points to the next delimiter in `ToTokenize`
    auto NextDelimiterIterator =
        min_element(DelimiterIndexes.begin(), DelimiterIndexes.end());

    // The distance call converts the iterator to a vector index in
    // `DelimiterIndexes` It's important to note here that `DelimiterIndexes`
    // contains string indexes And our current calculation is regarding vector
    // indexes which point to those string indexes.
    string NextDelimiter =
        Delimiters[distance(DelimiterIndexes.begin(), NextDelimiterIterator)];

    // We already know the next delimiter exists in `ToTokenize`
    // It's implied by the condition to enter this loop

    ToReturn.push_back(
        ToTokenize.substr(StartIndex, (*NextDelimiterIterator) - StartIndex));
    // Some delimiters are kept, some are discarded.
    // Where a delimiter is kept, it becomes its own token in the token stream.

    // This is shorthand to make the next line easier to read.
    // No additional code should be generated here.
    vector<string> const& Kepts = KeptDelimiters;

    if (std::find(Kepts.begin(), Kepts.end(), NextDelimiter) != Kepts.end()) {
      ToReturn.push_back(NextDelimiter);
    }  // We wish to keep the delimiters in the final result.
    StartIndex = (*NextDelimiterIterator) + NextDelimiter.length();

    // Update DelimiterIndexes now that another delimiter has been processed.
    for (size_t Index = 0; Index < Delimiters.size(); Index++) {
      DelimiterIndexes[Index] = ToTokenize.find(Delimiters[Index], StartIndex);
    }
  }

  // Empty strings can appear
  // We pull those out here
  // Because they can throw off
  // the calculations in other
  // sections that assume they'll never
  // draw an empty token.
  ToReturn.erase(
      std::remove_if(ToReturn.begin(), ToReturn.end(),
                     [](const string& Str) -> bool { return Str.empty(); }),
      ToReturn.end());

  Log(std::cout, LOG, "Tokenizer output is");
  for (auto const& Token : ToReturn) {
    Log(std::cout, VALUE_OUTPUT, Token);
    Log(std::cout, VALUE_OUTPUT, "-|-|-|-|-|-");
  }

  return ToReturn;
}
