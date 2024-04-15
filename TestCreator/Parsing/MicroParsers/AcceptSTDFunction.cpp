#include "AcceptSTDFunction.h"

#include <iostream>
#include <vector>

#include "StringOperations.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/AdvancedLoggingWithBrackets.h"
#include "TestCreator/Parsing/MicroParsers/AcceptType.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"

auto AcceptSTDFunction(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<ParsedFunction> {
  ParsingLogging.Log(std::cout, true, "Beginning to accept STD function");
  ParsingLogging.IncreaseIndentationLevel();

  ParsedResult<std::string> FunctionType = AcceptType(FirstToken);

  if (!FunctionType.WasLegalInput) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false,
                       "Parsed function type was illegal as a type");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  std::string ToParse = FunctionType.Result;

  if (ToParse.find("std::function") == std::string::npos) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false,
                       "Parsed function type didn't contain std::function");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find("<") == std::string::npos) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false, "Parsed function didn't contain <");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find(">") == std::string::npos) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false, "Parsed function didn't contain >");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find("(") == std::string::npos) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false, "Parsed function didn't contain (");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find(")") == std::string::npos) {
    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, false, "Parsed function didn't contain )");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  std::string ReturnType;

  int ReturnTypeIndex = ToParse.find("<");
  ReturnTypeIndex++;

  while (ToParse[ReturnTypeIndex] != '(') {
    ReturnType += ToParse[ReturnTypeIndex];
    ReturnTypeIndex++;
  }

  std::vector<std::string> ArgumentTypes;
  std::string CurrentArgumentType = "";

  int ArgumentTypesIndex = ToParse.find("(");
  ArgumentTypesIndex++;

  while (ToParse[ArgumentTypesIndex] != ')') {
    if (ToParse[ArgumentTypesIndex] == ',') {
      ArgumentTypes.push_back(CurrentArgumentType);
      CurrentArgumentType = "";
    } else {
      CurrentArgumentType += ToParse[ArgumentTypesIndex];
    }
    ArgumentTypesIndex++;
  }

  // We now have to dump the last argument into our list if it exists
  if (CurrentArgumentType != "") {
    ArgumentTypes.push_back(CurrentArgumentType);
  }

  std::vector<bool> PartsWereLegal;

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
  PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "&"));

  ParsedResult<std::string> FunctionName = AcceptAnyToken(FirstToken);
  ParsingLogging.Log(std::cout, true, "Found function name");
  ParsingLogging.OutputValue(std::cout, FunctionName.Result);

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));

  ParsedFunction ToReturn(FunctionName.Result, ReturnType, ArgumentTypes);

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, true,
                     "Finished parsing std::function with following name, "
                     "return type, and argument types");
  ParsingLogging.OutputValue(std::cout, FunctionName.Result);
  ParsingLogging.OutputValue(std::cout, ReturnType);
  ParsingLogging.OutputValue(std::cout,
                             JoinVectorOfStrings(ArgumentTypes, ","));
  return {true, ToReturn};
}
