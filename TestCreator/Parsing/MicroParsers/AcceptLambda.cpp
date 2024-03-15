#include "AcceptLambda.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "Logging.h"
#include "StringOperations.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "VectorOperations.h"

auto GetCaptureAllLegalParametersSource() -> std::string {
  std::vector<std::string> Parameters;

  for (const auto& Parameter : LegalParameters) {
    Parameters.push_back(Parameter.first + " " + Parameter.second);
  }

  std::string ToReturn = "[" + JoinVectorOfStrings(Parameters, ", ");

  // Remove the final ", "
  ToReturn = ToReturn.substr(0, ToReturn.size() - 2);

  // Close the opening bracket
  ToReturn += "]";

  return ToReturn;
}

// This function returns the source of the lambda function AFTER
// it has been translated to work in the generated test
auto AcceptLambda(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  std::vector<bool> WereBracketsLegal;

  // Should capture zero values.
  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, SQUARE));
  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, SQUARE));

  // Now there's some trickery.
  // This lambda is written in a different context to the one it
  // will eventually run in.
  // So we will need to remove all of the "arguments" here
  // because they will be captured values later.
  // but we still wish to check that each type and identifier are legal.
  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));

  while (*FirstToken != ")") {
    std::string Type = AcceptAnyToken(FirstToken).Result;
    std::string Identifier = AcceptAnyToken(FirstToken).Result;

    // If this is NOT a legal pair
    if (LegalParameters.find(std::make_pair(Type, Identifier)) ==
        LegalParameters.end()) {
      auto TargetIdentifier = std::find_if(
          LegalParameters.begin(), LegalParameters.end(),
          [Identifier](const std::pair<std::string, std::string>& Item)
              -> bool { return Item.second == Identifier; });

      if (TargetIdentifier != LegalParameters.end()) {
        // TODO: This should go to an object that makes a real error message
        Log(std::cout, LOG,
            "Identifier " + Identifier +
                "called with the wrong type. It has type " +
                TargetIdentifier->first + ", not" + Type + ".");
      }

      else {
        Log(std::cout, LOG, "Identifier " + Identifier + " is not legal.");
      }

      return {false, ""};
    }

    // We don't know if this parameter is the last one yet.
    if (*FirstToken == ",") {
      AcceptAnyToken(FirstToken);
    }
  }

  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));

  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE));

  // The rest is the source code

  std::vector<std::string> InternalLambdaSource;

  std::size_t InitialBracketDepth = BracketAcceptor::GetBracketDepth();

  while (!(*FirstToken == "}" &&
           BracketAcceptor::GetBracketDepth() == InitialBracketDepth)) {
    InternalLambdaSource.push_back(*FirstToken);
    FirstToken++;
  }

  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));

  std::string FinalLambdaSource =
      GetCaptureAllLegalParametersSource() + "(){\n" +
      JoinVectorOfStrings(InternalLambdaSource, "\n");

  return {AllOf(WereBracketsLegal), FinalLambdaSource};
}
