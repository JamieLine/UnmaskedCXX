#include <algorithm>
#include <iterator>

#include "AcceptFunction.h"
#include "TestCreator/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Acceptors/AcceptSpecificString.h"
#include "TestCreator/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "VectorOperations.h"

auto AcceptSTDFunction(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedFunction> {
  // TODO(linej): Make this use AcceptType
  bool HadNamespace = AcceptSpecificString(FirstToken, "std");
  bool HadColons = AcceptSpecificString(FirstToken, ":") &&
                   AcceptSpecificString(FirstToken, ":");

  bool HadFunction = AcceptSpecificString(FirstToken, "function");

  std::vector<bool> HadLegalParameterPack;

  HadLegalParameterPack.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED));

  auto ReturnType = AcceptAnyToken(FirstToken);

  std::vector<ParsedResult<std::string>> ArgumentTypes;

  // Now we need zero or more argument types
  bool LegalOpeningBracket =
      true;  // The absence of such a bracket is entirely legal
  bool LegalClosingBracket = true;
  if (*FirstToken == "(") {
    LegalClosingBracket =
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
    while (*FirstToken != ")") {
      ArgumentTypes.push_back(AcceptAnyToken(FirstToken));
      if (*FirstToken == ",") {
        AcceptSpecificString(FirstToken, ",");
      }
    }
    LegalClosingBracket =
        BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  }

  HadLegalParameterPack.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED));

  bool HadLegalNameStart =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  bool HadAmpersand = AcceptSpecificString(FirstToken, "&");

  auto FunctionName = AcceptAnyToken(FirstToken);

  bool HadLegalNameEnd =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);

  ParsedFunction ToReturn = {
      .Name = FunctionName.Result,
      .ReturnType = ReturnType.Result,
      .ArgumentTypes = ExtractResults(ArgumentTypes),
  };

  bool WasLegalInput = HadNamespace && HadLegalNameEnd && HadLegalNameStart &&
                       HadAmpersand && HadColons && ReturnType.WasLegalInput &&
                       AllLegal(ArgumentTypes) && AllOf(HadLegalParameterPack);
}