#include "BracketAcceptor.h"

#include <iostream>

#include "Logging.h"
#include "TestCreator/Acceptors/AcceptSpecificString.h"

auto BracketAcceptor::AcceptOpeningBracket(TokenArray::iterator& FirstToken,
                                           BracketType Type) -> bool {
  bool GotToken = AcceptSpecificString(FirstToken, GetOpeningBrackets(Type));

  if (!GotToken) {
    return false;
  }

  CurrentlyOpenBrackets.push(Type);
  return true;
}

auto BracketAcceptor::AcceptClosingBracket(TokenArray::iterator& FirstToken,
                                           BracketType Type) -> bool {
  bool GotToken = AcceptSpecificString(FirstToken, GetClosingBrackets(Type));

  if (!GotToken) {
    return false;
  }

  // If this bracket isn't closing the most recently opened bracket
  if (CurrentlyOpenBrackets.top() != Type) {
    return false;
  }

  // At this point, we know that this is correct
  CurrentlyOpenBrackets.pop();
  return true;
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
      "Somehow BracketAcceptor::GetClosingBracket ended up trying to match a "
      "type that isn't a BracketType?");

  return "";
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
      "Somehow BracketAcceptor::GetOpeningBracket ended up trying to match a "
      "type that isn't a BracketType?");

  return "";
}

auto BracketAcceptor::GetBracketDepth() -> std::size_t {
  return CurrentlyOpenBrackets.size();
}