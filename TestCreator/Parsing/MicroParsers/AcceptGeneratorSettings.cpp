#include "AcceptGeneratorSettings.h"

#include "TestCreator/Parsing/Acceptors/AcceptAnyToken.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/GeneratorSettingDescriptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"
#include "VectorOperations.h"

auto AcceptSingleGeneratorSetting(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<GeneratorSettingDescriptor> {
  std::vector<bool> PartsWereLegal;

  GeneratorSettingDescriptor ToReturn;

  if (*FirstToken == "(") {
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED));
    PartsWereLegal.push_back(
        AcceptSpecificString(FirstToken, "GeneratorSettings"));
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, ROUNDED));
    ParsingLogging.Log(std::cout, true,
                       "Finished parsing GeneratorSettings cast");
    PrintVector(std::cout, PartsWereLegal);
  }

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE));
  while (*FirstToken != "}") {
    // Perform a full legality check on something of the form
    // ".{Identifier} = {Value}"
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "."));

    auto Identifier = AcceptAnyToken(FirstToken);
    ParsingLogging.Log(std::cout, Identifier.WasLegalInput,
                       "Pulled identifier");
    ParsingLogging.OutputValue(std::cout, Identifier.Result);
    PartsWereLegal.push_back(Identifier.WasLegalInput);

    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, "="));

    std::string Value = "";

    std::size_t StartingBracketDepth = BracketAcceptor::GetBracketDepth();

    // This has to check the depth because the value can contain a lambda which
    // doesn't need full parsing
    while (*FirstToken != "," &&
           (!(*FirstToken == "}" &&
              BracketAcceptor::GetBracketDepth() == StartingBracketDepth))) {
      auto Token = AcceptAnyToken(FirstToken);
      if (Token.Result == "return") {
        Token.Result += " ";
      }
      Value += Token.Result;
      PartsWereLegal.push_back(Token.WasLegalInput);

      if (Token.Result == "\"") {
        ParsingLogging.Log(std::cout, true, "WE HAVE A QUOTE");
      }

      ParsingLogging.Log(std::cout, Token.WasLegalInput, "Added part to Value");
      ParsingLogging.OutputValue(std::cout, Token.Result);
    }

    // if (*FirstToken == "}") {
    //   PartsWereLegal.push_back(
    //       BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));
    // }

    if (*FirstToken == ",") {
      PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));
    }

    ToReturn[Identifier.Result] = Value;
    ParsingLogging.Log(std::cout, true, "Pulled new value");
    ParsingLogging.OutputValue(std::cout, Value);
  }

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));

  if (*FirstToken == ",") {
    PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));
  }

  return {AllOf(PartsWereLegal), ToReturn};
}

auto AcceptGeneratorSettings(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<GeneratorSettingBunch> {
  // We might be given no generator settings, in which case this is the end of
  // the test definiton

  ParsingLogging.IncreaseIndentationLevel();
  ParsingLogging.Log(std::cout, true, "Beginning to accept Generator Settings");
  ParsingLogging.OutputValue(std::cout, *FirstToken);

  GeneratorSettingBunch ToReturn;
  std::vector<bool> PartsWereLegal;

  PartsWereLegal.push_back(
      BracketAcceptor::AcceptOpeningBracket(FirstToken, BRACE));

  if (*FirstToken == ")" || *FirstToken == "}") {
    ParsingLogging.Log(std::cout, true, "No Generator Settings found");

    if (*FirstToken == "}") {
      // AcceptSpecificString(FirstToken, "}");
      BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE);
    }

    ParsingLogging.DecreaseIndentationLevel();
    return {true, GeneratorSettingBunch()};
  }

  while (*FirstToken != "}") {
    auto Setting = AcceptSingleGeneratorSetting(FirstToken);
    ToReturn.Settings.push_back(Setting.Result);

    PartsWereLegal.push_back(Setting.WasLegalInput);

    if (*FirstToken == ",") {
      PartsWereLegal.push_back(AcceptSpecificString(FirstToken, ","));
    }
  }

  // PartsWereLegal.push_back(
  //     BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));
  if (ToReturn.Settings.size() > 0) {
    PartsWereLegal.push_back(
        BracketAcceptor::AcceptClosingBracket(FirstToken, BRACE));
  }

  bool WasLegal = AllOf(PartsWereLegal);
  ParsingLogging.Log(std::cout, WasLegal,
                     "Finished accepting Generator Settings");
  ParsingLogging.DecreaseIndentationLevel();
  return {WasLegal, ToReturn};
}
