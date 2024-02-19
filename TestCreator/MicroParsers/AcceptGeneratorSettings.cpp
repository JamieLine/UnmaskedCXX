#include "AcceptGeneratorSettings.h"

#include "TestCreator/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Acceptors/AcceptSpecificString.h"
#include "TestCreator/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"
#include "VectorOperations.h"

auto AcceptSingleGeneratorSetting(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  std::vector<bool> PartsWereLegal;

  std::string ToReturn = "";

  // Might have a (GeneratorSettings) cast
  // Need to generalise this
  // TODO: that
  if (*FirstToken == "(") {
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
    PartsWereLegal.push_back(
        AcceptSpecificString(FirstToken, "GeneratorSettings"));
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));
  }

  ToReturn += "(GeneratorSettings) {";

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE));
  while (*FirstToken != "}") {
    // Perform a full legality check on something of the form
    // ".{Identifier} = {Value}"
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "."));

    auto Identifier = AcceptAnyToken(FirstToken);
    PartsWereLegal.push_back(Identifier.WasLegalInput);

    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "="));

    std::string Value = "";

    while (*FirstToken != "," && *FirstToken != "}") {
      auto Token = AcceptAnyToken(FirstToken);
      Value += Token.Result;
      PartsWereLegal.push_back(Token.WasLegalInput);
    }

    if (*FirstToken == ",") {
      PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));
    }

    ToReturn += "." + Identifier.Result + " = " + Value + ",";
  }

  ToReturn += "}";

  if (*FirstToken == ",") {
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));
    ToReturn += ",";
  }

  return {AllOf(PartsWereLegal), ToReturn};
}

auto AcceptGeneratorSettings(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  // We might be given no generator settings, in which case this is the end of
  // the test definiton
  if (*FirstToken == ")") {
    return {true, ""};
  }

  std::string ToReturn = "";
  std::vector<bool> PartsWereLegal;

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE));

  while (*FirstToken != "}") {
    auto Setting = AcceptSingleGeneratorSetting(FirstToken);
    ToReturn += Setting.Result + "\n";
    PartsWereLegal.push_back(Setting.WasLegalInput);
  }

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));

  return {AllOf(PartsWereLegal), "{" + ToReturn + "}"};
}