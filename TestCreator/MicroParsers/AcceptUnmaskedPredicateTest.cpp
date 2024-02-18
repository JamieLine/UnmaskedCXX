#include "AcceptUnmaskedPredicateTest.h"

#include <algorithm>

#include "TestCreator/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Acceptors/AcceptSpecificString.h"
#include "TestCreator/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"

auto AcceptUnmaskedPredicateTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<int> {
  bool WasLegallyTitled =
      AcceptSpecificString(FirstToken, "UnmaskedPredicateTest");

  BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  std::vector<bool> HadLegalCommas;
  std::vector<T> HadLegalGeneratorSettings;

  // The types here are intentionally wrong for prototyping
  // This way they will cause compiler errors if they aren't fixed
  // This is a sketch

  T HadFunction = AcceptFunction(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ",");
  T HadLambda = AcceptLambda(FirstToken);
  HadLegalCommas.push_back(AcceptSpecificString(FirstToken, ",");

  // Now we need zero or more generator settings
  bool HadOpenBracket = BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE);
  
  while (*FirstToken == "(") {
    HadLegalGeneratorSettings.push_back(AcceptGeneratorSettings(FirstToken));
  }

  bool HadCloseBracket = BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE);

  bool WasLegal = WasLegallyTitled &&
    std::all_of(HadLegalCommas.begin(), HadLegalCommas.end(), [](bool B) {
    return B; }) &&
    std::all_of(HadLegalGeneratorSettings.begin(), HadLegalGeneratorSettings.end(), [](bool B) {
    return B; }) &&
    HadFunction &&
    HadLambda &&
    HadOpenBracket &&
    HadCloseBracket;

  return WasLegal;
}