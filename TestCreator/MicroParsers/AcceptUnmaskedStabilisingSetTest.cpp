#include "AcceptUnmaskedStabilisingSetTest.h"

#include "TestCreator/Acceptors/AcceptSpecificString.h"
#include "TestCreator/MicroParsers/AcceptGeneratorSettings.h"
#include "TestCreator/MicroParsers/AcceptLambda.h"
#include "TestCreator/MicroParsers/AcceptSTDFunction.h"
#include "TestCreator/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "VectorOperations.h"

auto AcceptUnmaskedStabilisingSetTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest> {
  std::vector<bool> HadLegalCommas;

  bool WasLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedStabilisingSetTest");

  bool HadStartingBracket =
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);

  ParsedResult<ParsedFunction> TestedFunction = AcceptSTDFunction(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ","));

  ParsedResult<std::string> ParsedGeneratorSettings =
      AcceptGeneratorSettings(FirstToken);

  bool HadFinalBracket =
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
  bool HadSemiColon = AcceptSpecificString(FirstToken, ";");

  const std::string& TestedReturnType = TestedFunction.Result.ReturnType;

  // This is the shortcut we gain with this type of test
  std::string Lambda = "() [" + GetCaptureAllLegalParametersSource() + "] { " +
                       "std::all_of(ReturnedValues.begin(), " +
                       "ReturnedValues.end(), " + "(" + TestedReturnType +
                       " V)[std::vector<" + TestedReturnType +
                       "> ReturnedValues]{ return V == ReturnedValues[0]; }})";

  ParsedResult<ParsedUnmaskedPredicateTest> ToReturn = {
      AllOf(HadLegalCommas) && WasLegallyTitled && HadStartingBracket &&
          HadFinalBracket && HadSemiColon &&
          ParsedGeneratorSettings.WasLegalInput && TestedFunction.WasLegalInput,
      {
          .TestedFunction = TestedFunction.Result,
          .PredicateSource = Lambda,
          .GeneratorSource = ParsedGeneratorSettings.Result,
      }};

  return ToReturn;
}