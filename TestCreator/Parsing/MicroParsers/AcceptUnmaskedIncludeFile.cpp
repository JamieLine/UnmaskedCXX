
#include "AcceptUnmaskedIncludeFile.h"

#include <random>

#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TestCreationStatus.h"
#include "VectorOperations.h"

ParsedResult<std::string> AcceptUnmaskedIncludeFile(
    TokenArray::iterator& FirstToken) {
  std::vector<bool> PartsWereLegal;

  // Accept the marker, the opening bracket, and the opening quote
  PartsWereLegal.push_back(
      AcceptSpecificString(FirstToken, "UnmaskedIncludeFile"));
  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
  PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "\""));

  std::string Address = "";

  while (*FirstToken != "\"") {
    auto NextToken = AcceptAnyToken(FirstToken);
    Address += NextToken.Result;
    PartsWereLegal.push_back(NextToken.WasLegalInput);
  }

  PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "\""));
  PartsWereLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));
  PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ";"));

  std::string Result = "#include \"" + Address + "\"";

  return {AllOf(PartsWereLegal), Result};
}
