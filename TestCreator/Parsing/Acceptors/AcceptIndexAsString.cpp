#include "AcceptIndexAsString.h"

#include <algorithm>

auto AcceptIndexAsString(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<std::string> {
  // Its an index if its an integer at least 0
  // which is equivalent to every char being a digit

  std::string& InputString = *FirstToken;
  FirstToken++;

  bool IsLegal = !InputString.empty() &&
                 std::all_of(InputString.begin(), InputString.end(),
                             [](unsigned char C) { return std::isdigit(C); });

  return {.WasLegalInput = IsLegal, .Result = InputString};
}
