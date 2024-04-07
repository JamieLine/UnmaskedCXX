#include "AcceptLambda.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>

#include "StringOperations.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/AdvancedLoggingWithBrackets.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "VectorOperations.h"

auto GetCaptureAllLegalParametersSource() -> std::string {
  // std::vector<std::string> Parameters;

  /*  std::transform(LegalParameters.begin(), LegalParameters.end(),
                   std::back_inserter(Parameters),
                   [](const std::pair<std::string, std::string>& Item) {
                     // return Item.first + " " + Item.second;
                     // Lambda captures don't need a type.
                     //return Item.second;
                   });*/

  //  std::string ToReturn = "[" + JoinVectorOfStrings(Parameters, ", ");

  // Close the opening bracket
  // ToReturn += "]";

  std::string ToReturn = "[&]";

  return ToReturn;
}

// This function returns the source of the lambda function AFTER
// it has been translated to work in the generated test
auto AcceptLambda(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  ParsingLogging.Log(std::cout, true, "Beginning to accept lambda");
  ParsingLogging.IncreaseIndentationLevel();

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
    // Note, we're just doing this as a validation step. It doesn't matter which
    // parameters are specified as all will be given to the final program.
    // Therefore, we don't need to push anything we read here out of this
    // function.
    std::string Type = AcceptAnyToken(FirstToken).Result;
    std::string Identifier = AcceptAnyToken(FirstToken).Result;

    ParsingLogging.Log(std::cout, true,
                       "Found Type and Indentifier of parameter");
    ParsingLogging.OutputValue(std::cout, Type);
    ParsingLogging.OutputValue(std::cout, Identifier);

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

      ParsingLogging.Log(
          std::cout, false,
          "Lambda has a parameter which is not within the specified legal "
          "parameters. Check the names and types of the parameters.");
      ParsingLogging.DecreaseIndentationLevel();
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
    ParsingLogging.Log(std::cout, true, "Reading in value to parse lambda");
    ParsingLogging.OutputValue(std::cout, *FirstToken);
    ParsingLogging.Log(std::cout, true, "BracketAcceptor stack is");
    ParsingLogging.OutputBracketAcceptorStack(std::cout);
    InternalLambdaSource.push_back(*FirstToken);

    // We're ignoring the angled brackets as they could be a template or
    // inequality signs (A < B, B > A)

    if (*FirstToken == "(") {
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
    } else if (*FirstToken == "{") {
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE);
    } else if (*FirstToken == "[") {
      BracketAcceptor::AcceptOpeningBracket(FirstToken, SQUARE);
    } /*else if (*FirstToken == "<") {
      BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED);
    } */
    else if (*FirstToken == ")") {
      BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED);
    } else if (*FirstToken == "}") {
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE);
    } else if (*FirstToken == "]") {
      BracketAcceptor::AcceptClosingBracket(FirstToken, SQUARE);
    } /* else if (*FirstToken == ">") {
      BracketAcceptor::AcceptClosingBracket(FirstToken, ANGLED);
    } */
    else {
      FirstToken++;
    }

    ParsingLogging.Log(std::cout, true, "Pulled a token for lambda.");
    ParsingLogging.OutputBracketAcceptorStack(std::cout);
  }

  WereBracketsLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));

  std::string FinalLambdaSource =
      GetCaptureAllLegalParametersSource() + "(){\n" +
      JoinVectorOfStrings(InternalLambdaSource, "\n") + "}";

  bool WasLegal = AllOf(WereBracketsLegal);

  PrintVector(std::cout, WereBracketsLegal);

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, WasLegal, "Finished parsing lambda");

  return {AllOf(WereBracketsLegal), FinalLambdaSource};
}
