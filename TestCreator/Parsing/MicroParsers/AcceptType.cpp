#include "AcceptType.h"

#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"
#include "VectorOperations.h"

auto AcceptType(TokenArray::iterator& FirstToken) -> ParsedResult<std::string> {
  ParsingLogging.Log(std::cout, true, "Beginning to accept type");
  ParsingLogging.IncreaseIndentationLevel();

  ParsedResult<std::string> Name = AcceptAnyToken(FirstToken);
  ParsingLogging.Log(std::cout, true, "Pulled first identifier in type");
  ParsingLogging.OutputValue(std::cout, Name.Result);

  std::vector<bool> PartsWereLegal;

  while (*FirstToken == ":") {
    ParsingLogging.Log(std::cout, true, "Trying to accept :: in type name");
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ":"));
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ":"));

    ParsedResult<std::string> NextPart = AcceptAnyToken(FirstToken);
    ParsingLogging.Log(std::cout, true, "Pulling next part of type name");
    ParsingLogging.OutputValue(std::cout, NextPart.Result);
    Name.Result += "::" + NextPart.Result;
    Name.WasLegalInput = Name.WasLegalInput && NextPart.WasLegalInput;
  }

  if (*FirstToken == "<") {
    ParsingLogging.Log(std::cout, true,
                       "Beginning to read first template type");
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED));
    ParsedResult<std::string> FirstTemplateType = AcceptType(FirstToken);
    PartsWereLegal.push_back(FirstTemplateType.WasLegalInput);

    Name.Result += "<" + FirstTemplateType.Result;
    Name.WasLegalInput = Name.WasLegalInput && FirstTemplateType.WasLegalInput;

    // While not at the end of the specification
    while (*FirstToken != ">") {
      // If we are about to hit std::function argument types or similar
      if (*FirstToken == "(") {
        ParsingLogging.Log(std::cout, true,
                           "Beginning to accept template types after (");
        PartsWereLegal.push_back(
            BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
        ParsedResult<std::string> NextTemplateType = AcceptType(FirstToken);

        ParsingLogging.Log(std::cout, NextTemplateType.WasLegalInput,
                           "Pulled in template type after (");
        ParsingLogging.OutputValue(std::cout, NextTemplateType.Result);

        Name.Result += "(" + NextTemplateType.Result;
        Name.WasLegalInput =
            Name.WasLegalInput && NextTemplateType.WasLegalInput;
      }

      if (*FirstToken == ")") {
        ParsingLogging.Log(std::cout, true,
                           "Ending the template types after ), not expecting "
                           "more types in this template pack");
        PartsWereLegal.push_back(
            BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));
        Name.Result += ")";
      }

      if (*FirstToken == ",") {
        ParsingLogging.Log(std::cout, true,
                           "Beginning to accept another type in the template");
        PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));

        ParsedResult<std::string> NextTemplateType = AcceptType(FirstToken);
        ParsingLogging.Log(std::cout, NextTemplateType.WasLegalInput,
                           "Pulled in template type after ,");
        ParsingLogging.OutputValue(std::cout, NextTemplateType.Result);

        Name.Result += "," + NextTemplateType.Result;
        Name.WasLegalInput =
            Name.WasLegalInput && NextTemplateType.WasLegalInput;
      }
    }

    PartsWereLegal.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, ANGLED));
    ParsingLogging.Log(std::cout, PartsWereLegal.back(),
                       "Looked for > to end template specification");
    Name.Result += ">";
  }

  bool WasLegal = Name.WasLegalInput && AllOf(PartsWereLegal);

  ParsingLogging.DecreaseIndentationLevel();
  ParsingLogging.Log(std::cout, WasLegal, "Finished accepting type");
  ParsingLogging.OutputValue(std::cout, Name.Result);

  return {WasLegal, Name.Result};
}
