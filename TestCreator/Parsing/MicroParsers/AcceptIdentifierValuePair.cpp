#include "AcceptIdentifierValuePair.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/Task.h"

auto AcceptIdentifierValuePair(TokenArray::RawTokenIterator& FirstToken) -> ParsedResult<std::pair<std::string, std::string>> {
  bool AcceptedLeadingDot = AcceptSpecificString(FirstToken, ".");

  if (!AcceptedLeadingDot) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Couldn't find '.' before the parameter's identifier. Expected '.', and found " + PreviousToken);
    return {false, std::make_pair("", "")};
  }

  ParsedResult<std::string> Identifier = AcceptAnyToken(FirstToken);

  if (!Identifier.WasLegalInput) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Was given an illegal identifier. Found " + PreviousToken);
    return {false, std::make_pair("", "")};
  }

  bool AcceptedEquals = AcceptSpecificString(FirstToken, "=");

  if (!AcceptedEquals) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Couldn't find '=' as part of parameter assignment. Expected '=', and found " + PreviousToken);
    return {false, std::make_pair("", "")};
  }

  //TODO: THIS IS WRONG, IT NEEDS TO HANDLE MULTI-TOKEN VALUES, SEE THE SECTION IN AcceptGeneratorSettings labelled as THE RIGHT WAY
  // This can only be true for now
  ParsedResult<std::string> Value = AcceptAnyToken(FirstToken);

  return {true, std::make_pair(Identifier.Result, Value.Result)};
}
