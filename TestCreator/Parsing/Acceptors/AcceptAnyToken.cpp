#include "AcceptAnyToken.h"

#include <iostream>

#include "Logging.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
auto AcceptAnyToken(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string> {
  if (*FirstToken == "{") {
    ParsingLogging::Log(std::cout, true, "Parsing { as any token");
    return {BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE), "{"};
  } else if (*FirstToken == "[") {
    ParsingLogging::Log(std::cout, true, "Parsing [ as any token");
    return {BracketAcceptor::AcceptOpeningBracket(FirstToken, SQUARE), "["};
  } else if (*FirstToken == "(") {
    ParsingLogging::Log(std::cout, true, "Parsing ( as any token");
    return {BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED), "("};
  } else if (*FirstToken == "}") {
    ParsingLogging::Log(std::cout, true, "Parsing } as any token");
    return {BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE), "}"};
  } else if (*FirstToken == "]") {
    ParsingLogging::Log(std::cout, true, "Parsing ] as any token");
    return {BracketAcceptor::AcceptClosingBracket(FirstToken, SQUARE), "]"};
  } else if (*FirstToken == ")") {
    ParsingLogging::Log(std::cout, true, "Parsing ) as any token");
    return {BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED), ")"};
  } else {
    ParsingLogging::Log(
        std::cout, true,
        "Accepting any token and it was not a bracket (or was <>)");
    std::string Input = *FirstToken;
    FirstToken++;
    return {.WasLegalInput = true, .Result = Input};
  }
}
