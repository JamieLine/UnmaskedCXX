#include "AcceptIndexValuePair.h"

#include "AcceptAnyToken.h"
#include "AcceptIndexAsString.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"

auto AcceptIndexValuePair(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::pair<std::string, std::string>> {
  ParsedResult<std::string> Index = AcceptIndexAsString(FirstToken);

  bool HadComma = AcceptSpecificString(FirstToken, ",");

  ParsedResult<std::string> Value = AcceptAnyToken(FirstToken);

  std::pair<std::string, std::string> ToReturn = {Index.Result, Value.Result};

  return {
      .WasLegalInput = Index.WasLegalInput && HadComma && Value.WasLegalInput,
      .Result = ToReturn};
}
