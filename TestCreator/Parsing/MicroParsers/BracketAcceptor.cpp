#include "BracketAcceptor.h"

#include <iostream>

#include "Logging.h"
#include "StringOperations.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "VectorOperations.h"

std::vector<BracketType> BracketAcceptor::CurrentlyOpenBrackets;

auto BracketAcceptor::AcceptOpeningBracket(TokenArray::iterator& FirstToken,
                                           BracketType Type) -> bool {
  ParsingLogging::Log(std::cout, true,
                      "BracketAcceptor is taking in a new bracket");
  bool GotToken = AcceptSpecificString(FirstToken, GetOpeningBrackets(Type));

  if (!GotToken) {
    ParsingLogging::Log(std::cout, false,
                        "BracketAcceptor didn't get the expected bracket");
    return false;
  }

  CurrentlyOpenBrackets.push_back(Type);
  return true;
}

auto BracketAcceptor::AcceptClosingBracket(TokenArray::iterator& FirstToken,
                                           BracketType Type) -> bool {
  bool GotToken = AcceptSpecificString(FirstToken, GetClosingBrackets(Type));

  ParsingLogging::OutputBracketAcceptorStack(std::cout);

  if (!GotToken) {
    ParsingLogging::Log(std::cout, false,
                        "BracketAcceptor tried to get a closing bracket but "
                        "found the wrong token. Expected first, got second.");
    ParsingLogging::OutputValue(std::cout, GetClosingBrackets(Type));
    ParsingLogging::OutputValue(std::cout, *(FirstToken - 1));
    return false;
  }

  // If this bracket isn't closing the most recently opened bracket
  if (CurrentlyOpenBrackets.back() != Type) {
    ParsingLogging::Log(
        std::cout, false,
        "BracketAcceptor tried to get a closing bracket, got the expected "
        "type, but that doesn't close the most recently opened bracket. Tried "
        "to close first with second.");
    ParsingLogging::OutputValue(
        std::cout, GetOpeningBrackets(CurrentlyOpenBrackets.back()));
    ParsingLogging::OutputValue(std::cout, GetClosingBrackets(Type));
    return false;
  }

  ParsingLogging::Log(std::cout, true, "BracketAcceptor closed bracket.");
  ParsingLogging::OutputValue(
      std::cout, GetOpeningBrackets(Type) + GetClosingBrackets(Type));
  CurrentlyOpenBrackets.pop_back();

  ParsingLogging::OutputBracketAcceptorStack(std::cout);

  return true;
}

// This would be constexpr if that were legal for strings.
auto BracketAcceptor::GetOpeningBrackets(BracketType Type) -> std::string {
  switch (Type) {
    case ROUNDED:
      return "(";
    case SQUARE:
      return "[";
    case ANGLED:
      return "<";
    case BRACE:
      return "{";
  }

  Log(std::cout, ERROR,
      "Somehow BracketAcceptor::GetOpeningBrackets ended up trying to match a "
      "type that isn't a BracketType?");

  return "";
}

// This would be constexpr if that were legal for strings.
auto BracketAcceptor::GetClosingBrackets(BracketType Type) -> std::string {
  switch (Type) {
    case ROUNDED:
      return ")";
    case SQUARE:
      return "]";
    case ANGLED:
      return ">";
    case BRACE:
      return "}";
  }

  Log(std::cout, ERROR,
      "Somehow BracketAcceptor::GetClosingBrackets ended up trying to match a "
      "type that isn't a BracketType?");

  return "";
}

auto BracketAcceptor::GetBracketDepth() -> std::size_t {
  return CurrentlyOpenBrackets.size();
}
