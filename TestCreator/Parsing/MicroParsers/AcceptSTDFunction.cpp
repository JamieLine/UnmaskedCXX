#include "AcceptSTDFunction.h"

#include <iostream>
#include <system_error>
#include <vector>

#include "Logging.h"
#include "StringOperations.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/AcceptType.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "VectorOperations.h"

auto AcceptSTDFunction(TokenArray::iterator &FirstToken)
    -> ParsedResult<ParsedFunction> {
  ParsingLogging::Log(std::cout, true, "Beginning to accept STD function");
  ParsingLogging::IncreaseIndentationLevel();

  ParsedResult<std::string> FunctionType = AcceptType(FirstToken);

  if (!FunctionType.WasLegalInput) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false,
                        "Parsed function type was illegal as a type");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  std::string ToParse = FunctionType.Result;

  if (ToParse.find("std::function") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false,
                        "Parsed function type didn't contain std::function");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find("<") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false, "Parsed function didn't contain <");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find(">") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false, "Parsed function didn't contain >");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find("(") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false, "Parsed function didn't contain (");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  if (ToParse.find(")") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false, "Parsed function didn't contain )");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }

  /*if (ToParse.find("&") == std::string::npos) {
    ParsingLogging::DecreaseIndentationLevel();
    ParsingLogging::Log(std::cout, false, "Parsed function didn't contain &");

    ParsedFunction ToReturn("", "", {});
    return {false, ToReturn};
  }*/

  std::vector<char> Delimiters = {',', '{', '}', '[', ']', '<', '>', '&'};

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

  while (ToParse[ArgumentTypesIndex] != ')') {
    if (ToParse[ArgumentTypesIndex] == ',') {
      ArgumentTypes.push_back(CurrentArgumentType);
      CurrentArgumentType = "";
    } else {
      CurrentArgumentType += ToParse[ArgumentTypesIndex];
    }
    ArgumentTypesIndex++;
  }

  std::vector<bool> PartsWereLegal;

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
  PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "&"));

  ParsedResult<std::string> FunctionName = AcceptAnyToken(FirstToken);
  ParsingLogging::Log(std::cout, true, "Found function name");
  ParsingLogging::OutputValue(std::cout, FunctionName.Result);

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));

  ParsedFunction ToReturn(FunctionName.Result, ReturnType, ArgumentTypes);

  ParsingLogging::DecreaseIndentationLevel();
  ParsingLogging::Log(std::cout, true,
                      "Finished parsing std::function with following name, "
                      "return type, and argument types");
  ParsingLogging::OutputValue(std::cout, FunctionName.Result);
  ParsingLogging::OutputValue(std::cout, ReturnType);
  ParsingLogging::OutputValue(std::cout,
                              JoinVectorOfStrings(ArgumentTypes, ","));
  return {true, ToReturn};
}
/*
auto AcceptSTDFunction(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedFunction> {
  // TODO(linej): Make this use AcceptType

  bool HadNamespace = AcceptSpecificString(FirstToken, "std::function");
  ParsingLogging::Log(std::cout, HadNamespace,
                      "Looked for \"std::function\" literal");

  std::vector<bool> HadLegalParameterPack;

  HadLegalParameterPack.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED));
  ParsingLogging::Log(std::cout, HadLegalParameterPack.back(),
                      "Looked for < in template specification");

  auto ReturnType = AcceptType(FirstToken);
  ParsingLogging::Log(std::cout, ReturnType.WasLegalInput,
                      "Looked for return type");

  std::vector<ParsedResult<std::string>> ArgumentTypes;
  std::vector<bool> LegalCommasBetweenArgs;

  // Now we need zero or more argument types
  bool LegalOpeningBracket =
      true;  // The absence of such a bracket is entirely legal
  bool LegalClosingBracket = true;
  if (*FirstToken == "(") {
    LegalOpeningBracket =
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
    ParsingLogging::Log(std::cout, true, "Found ( before argument types");
    while (*FirstToken != ")") {
      ArgumentTypes.push_back(AcceptType(FirstToken));
      ParsingLogging::Log(std::cout, ArgumentTypes.back().WasLegalInput,
                          "Looked for argument type");
      if (*FirstToken == ",") {
        LegalCommasBetweenArgs.push_back(AcceptSpecificString(FirstToken, ","));
        ParsingLogging::Log(std::cout, LegalCommasBetweenArgs.back(),
                            "Looked for comma between argument types");
      }
    }
    LegalClosingBracket =
        BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
    ParsingLogging::Log(std::cout, LegalClosingBracket,
                        "Looked for closing bracket after argument types");
  }

  HadLegalParameterPack.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, ANGLED));
  ParsingLogging::Log(std::cout, HadLegalParameterPack.back(),
                      "Looked for > at end of template specification");

  bool HadLegalNameStart =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  ParsingLogging::Log(std::cout, HadLegalNameStart,
                      "Looked for ( before the name of the function");
  bool HadAmpersand = AcceptSpecificString(FirstToken, "&");
  ParsingLogging::Log(std::cout, HadAmpersand,
                      "Looked for & before the name of the function");

  auto FunctionName = AcceptAnyToken(FirstToken);
  ParsingLogging::Log(std::cout, FunctionName.WasLegalInput,
                      "Looked for function name");
  ParsingLogging::OutputValue(std::cout, FunctionName.Result);

  bool HadLegalNameEnd =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  ParsingLogging::Log(std::cout, HadLegalNameEnd,
                      "Looked for ) after the name of the function");

  ParsedFunction ToReturn(FunctionName.Result, ReturnType.Result,
                          ExtractResults(ArgumentTypes));

  bool WasLegalInput =
      HadNamespace && HadLegalNameEnd && HadLegalNameStart && HadAmpersand &&
      ReturnType.WasLegalInput && AllLegal(ArgumentTypes) &&
      AllOf(HadLegalParameterPack) && AllOf(LegalCommasBetweenArgs);

  return {WasLegalInput, ToReturn};
}*/
