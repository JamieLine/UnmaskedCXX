#include "AcceptUnmaskedStabilisingSetTest.h"

#include <iostream>

#include "Logging.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/AcceptGeneratorSettings.h"
#include "TestCreator/Parsing/MicroParsers/AcceptLambda.h"
#include "TestCreator/Parsing/MicroParsers/AcceptSTDFunction.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "VectorOperations.h"

auto AcceptUnmaskedStabilisingSetTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest> {
  ParsingLogging::Log(std::cout, true,
                      "Beginning to parse UnmaskedStabilisingSetTest");
  ParsingLogging::IncreaseIndentationLevel();

  std::vector<bool> HadLegalCommas;

  bool WasLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedStabilisingSetTest");

  ParsingLogging::Log(std::cout, WasLegallyTitled, "Parsed title");

  bool HadStartingBracket =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);

  ParsingLogging::Log(std::cout, HadStartingBracket, "Parsed starting bracket");

  ParsedResult<ParsedFunction> TestedFunction =
      ParsingLogging::IndentForSubTask(std::cout, "Parsing STD function",
                                       AcceptSTDFunction, FirstToken);

  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));
  ParsingLogging::Log(std::cout, HadLegalCommas.back(), "Parsed comma");

  ParsedResult<GeneratorSettingBunch> ParsedGeneratorSettings =
      ParsingLogging::IndentForSubTask(std::cout, "Parsing Generator Settings",
                                       AcceptGeneratorSettings, FirstToken);

  bool HadFinalBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HadSemiColon = AcceptSpecificString(FirstToken, ";");

  const std::string& TestedReturnType = TestedFunction.Result.ReturnType;

  // This is the shortcut we gain with this type of test
  std::string Lambda = GetCaptureAllLegalParametersSource() + "() -> bool { " +
                       "return std::all_of(ReturnedValues.begin(), " +
                       "ReturnedValues.end(), " + "[&] (" + TestedReturnType +
                       " V) { return V == ReturnedValues[0]; });}";

  ParsedResult<ParsedUnmaskedPredicateTest> ToReturn = {
      AllOf(HadLegalCommas) && WasLegallyTitled && HadStartingBracket &&
          HadFinalBracket && HadSemiColon &&
          ParsedGeneratorSettings.WasLegalInput && TestedFunction.WasLegalInput,
      {
          .TestedFunction = TestedFunction.Result,
          .PredicateSource = Lambda,
          .GeneratorSettings = ParsedGeneratorSettings.Result,
      }};

  ParsingLogging::DecreaseIndentationLevel();
  ParsingLogging::Log(std::cout, ToReturn.WasLegalInput,
                      "Finished parsing UnmaskedStabilisingSetTest");

  return ToReturn;
}
