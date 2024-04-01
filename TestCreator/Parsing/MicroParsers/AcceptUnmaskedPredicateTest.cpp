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

auto AcceptUnmaskedPredicateTest(TokenArray::iterator& FirstToken)
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

  // The types here are intentionally wrong for prototyping
  // This way they will cause compiler errors if they aren't fixed
  // This is a sketch

  ParsedResult<ParsedFunction> TestedFunction = AcceptSTDFunction(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));
  ParsedResult<std::string> LambdaSource = AcceptLambda(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));

  // Now we need zero or more generator settings
  // bool HadOpenBracket =
  //    BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE);

  ParsedResult<GeneratorSettingBunch> ParsedGeneratorSettings =
      AcceptGeneratorSettings(FirstToken);

  // bool HadCloseBracket =
  //     BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE);

  bool HadFinalBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HadSemiColon = AcceptSpecificString(FirstToken, ";");

  bool WasLegal = WasLegallyTitled && HadStartingBracket && HadFinalBracket &&
                  AllOf(HadLegalCommas) && AllOf(HadLegalGeneratorSettings) &&
                  TestedFunction.WasLegalInput &&
                  LambdaSource.WasLegalInput
                  /*HadOpenBracket && HadCloseBracket */
                  && HadSemiColon;

  ParsedUnmaskedPredicateTest Result{
      .TestedFunction = TestedFunction.Result,
      .PredicateSource = LambdaSource.Result,
      // TODO: Should these generator settings go here or into a context
      // variable of type `TestCreationContext`?

      .GeneratorSettings = ParsedGeneratorSettings.Result,
  };

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, WasLegal,
                     "Finished parsing UnmaskedPredicateTest");

  return {WasLegal, Result};
}
