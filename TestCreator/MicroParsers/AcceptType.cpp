#include "AcceptType.h"

#include "StringOperations.h"
#include "TestCreator/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Acceptors/AcceptSpecificString.h"
#include "TestCreator/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "VectorOperations.h"

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
  // std::vector<std::pair<std::string, std::string

  // This pulls the first fragment, then we check if others are needed
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

  Name.WasLegalInput = Name.WasLegalInput && AllOf(OptionalPartLegality);
  return Name;
}