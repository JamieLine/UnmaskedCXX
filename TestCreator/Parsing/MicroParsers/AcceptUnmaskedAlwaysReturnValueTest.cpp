#include <iostream>

#include "AcceptUnmaskedStabilisingSetTest.h"
#include "AdvancedLogging.h"
#include "Logging.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
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

auto AcceptUnmaskedAlwaysReturnValueTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest> {
  ParsingLogging.Log(std::cout, true,
                     "Beginning to parse UnmaskedAlwaysReturnValueTest");
  ParsingLogging.IncreaseIndentationLevel();

  std::vector<bool> HadLegalCommas;

  bool WasLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedAlwaysReturnValueTest");

  ParsingLogging.Log(std::cout, WasLegallyTitled, "Parsed title");

  bool HadStartingBracket =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);

  ParsingLogging.Log(std::cout, HadStartingBracket, "Parsed starting bracket");

  ParsedResult<ParsedFunction> TestedFunction = ParsingLogging.IndentForSubTask(
      std::cout, "Parsing STD function", AcceptSTDFunction, FirstToken);

  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));
  ParsingLogging.Log(std::cout, HadLegalCommas.back(), "Parsed comma");

  std::string TargetValue;
  std::vector<bool> TargetValuePartsWereLegal;

  // If the next thing we read in is a comma, then the incoming specification
  // was missing this value entirely.
  if (*FirstToken == ",") {
    TargetValuePartsWereLegal.push_back(false);
  }

  else {
    ParsingLogging.Log(std::cout, true, "Beginning to parse target value");
    ParsingLogging.IncreaseIndentationLevel();

    auto StartingDepth = BracketAcceptor::GetBracketDepth();
    while (!(*FirstToken == ",") &&
           (BracketAcceptor::GetBracketDepth() == StartingDepth)) {
      auto NextPart = AcceptAnyToken(FirstToken);
      TargetValue += NextPart.Result;
      ParsingLogging.Log(std::cout, NextPart.WasLegalInput,
                         "Parsed " + NextPart.Result);
      TargetValuePartsWereLegal.push_back(NextPart.WasLegalInput);
    }

    ParsingLogging.DecreaseIndentationLevel();
    ParsingLogging.Log(std::cout, true, "Finished parsing target value");
  }

  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));
  ParsingLogging.Log(std::cout, HadLegalCommas.back(), "Parsed comma");

  ParsedResult<GeneratorSettingBunch> ParsedGeneratorSettings =
      ParsingLogging.IndentForSubTask(std::cout, "Parsing Generator Settings",
                                      AcceptGeneratorSettings, FirstToken);

  bool HadFinalBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HadSemiColon = AcceptSpecificString(FirstToken, ";");

  const std::string& TestedReturnType = TestedFunction.Result.ReturnType;

  // This is the shortcut we gain with this type of test
  std::string Lambda = GetCaptureAllLegalParametersSource() +
                       "() -> bool { return ReturnedValue == " + TargetValue +
                       "; }";

  ParsedResult<ParsedUnmaskedPredicateTest> ToReturn = {
      AllOf(HadLegalCommas) && WasLegallyTitled && HadStartingBracket &&
          HadFinalBracket && HadSemiColon &&
          ParsedGeneratorSettings.WasLegalInput &&
          TestedFunction.WasLegalInput && AllOf(TargetValuePartsWereLegal),
      {
          .TestedFunction = TestedFunction.Result,
          .PredicateSource = Lambda,
          .GeneratorSettings = ParsedGeneratorSettings.Result,
      }};

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, ToReturn.WasLegalInput,
                     "Finished parsing UnmaskedAlwaysReturnValueTest");

  return ToReturn;
}
