#include "AcceptAnyToken.h"

auto AcceptAnyToken(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  std::string Input = *FirstToken;
  FirstToken++;
  return {.WasLegalInput = true, .Result = Input};
}