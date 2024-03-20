#include "AcceptType.h"

#include "ParsingLogging.h"
#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"
#include "VectorOperations.h"

auto AcceptType(TokenArray::iterator& FirstToken) -> ParsedResult<std::string> {
  ParsingLogging::Log(std::cout, true, "Beginning to accept type");
  ParsingLogging::IncreaseIndentationLevel();

  ParsedResult<std::string> Name = AcceptAnyToken(FirstToken);
  ParsingLogging::Log(std::cout, true, "Pulled first identifier in type");
  ParsingLogging::OutputValue(std::cout, Name.Result);

  std::vector<bool> PartsWereLegal;

  while (*FirstToken == ":") {
    ParsingLogging::Log(std::cout, true, "Trying to accept :: in type name");
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ":"));
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ":"));

    ParsedResult<std::string> NextPart = AcceptAnyToken(FirstToken);
    ParsingLogging::Log(std::cout, true, "Pulling next part of type name");
    ParsingLogging::OutputValue(std::cout, NextPart.Result);
    Name.Result += "::" + NextPart.Result;
    Name.WasLegalInput = Name.WasLegalInput && NextPart.WasLegalInput;
  }

  if (*FirstToken == "<") {
    ParsingLogging::Log(std::cout, true,
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
        ParsingLogging::Log(std::cout, true,
                            "Beginning to accept template types after (");
        PartsWereLegal.push_back(
            BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
        ParsedResult<std::string> NextTemplateType = AcceptType(FirstToken);

        ParsingLogging::Log(std::cout, NextTemplateType.WasLegalInput,
                            "Pulled in template type after (");
        ParsingLogging::OutputValue(std::cout, NextTemplateType.Result);

        Name.Result += "(" + NextTemplateType.Result;
        Name.WasLegalInput =
            Name.WasLegalInput && NextTemplateType.WasLegalInput;
      }

      if (*FirstToken == ")") {
        ParsingLogging::Log(std::cout, true,
                            "Ending the template types after ), not expecting "
                            "more types in this template pack");
        PartsWereLegal.push_back(
            BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));
        Name.Result += ")";
      }

      if (*FirstToken == ",") {
        ParsingLogging::Log(std::cout, true,
                            "Beginning to accept another type in the template");
        PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));

        ParsedResult<std::string> NextTemplateType = AcceptType(FirstToken);
        ParsingLogging::Log(std::cout, NextTemplateType.WasLegalInput,
                            "Pulled in template type after ,");
        ParsingLogging::OutputValue(std::cout, NextTemplateType.Result);

        Name.Result += "," + NextTemplateType.Result;
        Name.WasLegalInput =
            Name.WasLegalInput && NextTemplateType.WasLegalInput;
      }
    }

    PartsWereLegal.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, ANGLED));
    ParsingLogging::Log(std::cout, PartsWereLegal.back(),
                        "Looked for > to end template specification");
    Name.Result += ">";
  }

  bool WasLegal = Name.WasLegalInput && AllOf(PartsWereLegal);

  ParsingLogging::DecreaseIndentationLevel();
  ParsingLogging::Log(std::cout, WasLegal, "Finished accepting type");
  ParsingLogging::OutputValue(std::cout, Name.Result);

  return {WasLegal, Name.Result};
}

/*
auto AcceptType(TokenArray::iterator& FirstToken) -> ParsedResult<std::string> {
  // The type could be either a single token
  // E.g. int
  // Or an identifier after a namespace
  // E.g. std::vector
  // Or a template
  // E.g. List<T>
  // where T is yet another type subject to all of these rules
  // or a list of types
  // E.g. std::pair<T1, T2>
  // Or a combination
  // E.g. std::vector<int>

  // The worst ones will be something like
  // std::vector<std::pair<std::string, std::string>>

  // This pulls the first fragment, then we check if others are needed
  //
  // TODO: Just rewrite this, it needs to become recursive because the original
  // language spec should be recursive
  auto Name = AcceptAnyToken(FirstToken);

  std::vector<bool> OptionalPartLegality;

  // If the first part was a namespace
  // and it may be one of many.
  while (*FirstToken == ":") {
    // double colon operator
    OptionalPartLegality.push_back(AcceptSpecificString(FirstToken, ":"));
    OptionalPartLegality.push_back(AcceptSpecificString(FirstToken, ":"));

    auto NamespacePart = AcceptAnyToken(FirstToken);

    Name.Result += "::" + NamespacePart.Result;
    Name.WasLegalInput = Name.WasLegalInput && NamespacePart.WasLegalInput;
  }

  // If the next part is a template argument
  if (*FirstToken == "<") {
    OptionalPartLegality.push_back(
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ANGLED));

    std::vector<ParsedResult<std::string>> TemplateTypes;

    // TODO: WHAT ABOUT TEMPLATE TYPES WITH ROUND BRACKETS LIKE
    // std::function<int(int, int)>

    // A template always has at least one type, but may have many
    bool IsFirstType = true;
    while (IsFirstType || *FirstToken == ",") {
      TemplateTypes.push_back(AcceptType(FirstToken));
    }

    OptionalPartLegality.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, ANGLED));

    OptionalPartLegality.push_back(AllLegal(TemplateTypes));

    auto TemplateTypeNames = ExtractResults(TemplateTypes);

    Name.Result += "<" + JoinVectorOfStrings(TemplateTypeNames, ",") + ">";
  }

  ParsingLogging::Log(std::cout, true, "Found type with name");
  ParsingLogging::OutputValue(std::cout, Name.Result);

  bool WasLegal = Name.WasLegalInput && AllOf(OptionalPartLegality);
  ParsingLogging::Log(
      std::cout, WasLegal,
      "Accepting type name legality and optional part legality vector");
  ParsingLogging::OutputValue(std::cout, std::to_string(Name.WasLegalInput));
  PrintVector(std::cout, OptionalPartLegality);

  Name.WasLegalInput = Name.WasLegalInput && AllOf(OptionalPartLegality);
  return Name;
}*/
