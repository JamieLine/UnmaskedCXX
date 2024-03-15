#include "AcceptUnmaskedSetParameter.h"

#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"

auto AcceptUnmaskedSetParameter(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::pair<std::string, std::string>> {
  bool IsLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedSetParameter");

  bool HasOpeningBracket =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);

  // TODO: This should probably check that this is a parameter name, and the
  // value is of suitable type

  ParsedResult<std::string> ParameterName = AcceptAnyToken(FirstToken);
  bool HasComma = AcceptSpecificString(FirstToken, ",");
  ParsedResult<std::string> ParameterValue = AcceptAnyToken(FirstToken);

  bool HasClosingBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HasSemiColon = AcceptSpecificString(FirstToken, ";");

  bool WasAllLegal = IsLegallyTitled && HasOpeningBracket &&
                     ParameterName.WasLegalInput && HasComma &&
                     ParameterValue.WasLegalInput && HasClosingBracket &&
                     HasSemiColon;

  return {WasAllLegal,
          std::make_pair(ParameterName.Result, ParameterValue.Result)};
}
