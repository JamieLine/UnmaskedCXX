#include "AcceptUnmaskedPredicateTest.h"

#include <algorithm>

#include "AcceptGeneratorSettings.h"
#include "AcceptSTDFunction.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/AdvancedLoggingWithBrackets.h"
#include "TestCreator/Parsing/MicroParsers/AcceptLambda.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "VectorOperations.h"

auto AcceptUnmaskedPredicateTest(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest> {
  ParsingLogging.Log(std::cout, true,
                     "Beginning to parse UnmaskedPredicateTest");
  ParsingLogging.IncreaseIndentationLevel();

  bool WasLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedPredicateTest");

  bool HadStartingBracket =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  std::vector<bool> HadLegalCommas;
  std::vector<bool> HadLegalGeneratorSettings;

  ParsedResult<ParsedFunction> TestedFunction = AcceptSTDFunction(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));
  ParsedResult<std::string> LambdaSource = AcceptLambda(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));

  // Now we need zero or more generator settings
  ParsedResult<GeneratorSettingBunch> ParsedGeneratorSettings =
      AcceptGeneratorSettings(FirstToken);

  bool HadFinalBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HadSemiColon = AcceptSpecificString(FirstToken, ";");

  bool WasLegal = WasLegallyTitled && HadStartingBracket && HadFinalBracket &&
                  AllOf(HadLegalCommas) && AllOf(HadLegalGeneratorSettings) &&
                  TestedFunction.WasLegalInput && LambdaSource.WasLegalInput &&
                  HadSemiColon;

  ParsedUnmaskedPredicateTest Result{
      .TestedFunction = TestedFunction.Result,
      .PredicateSource = LambdaSource.Result,

      .GeneratorSettings = ParsedGeneratorSettings.Result,
  };

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, WasLegal,
                     "Finished parsing UnmaskedPredicateTest");

  return {WasLegal, Result};
}
