#ifndef PARSINGLOGGING_H
#define PARSINGLOGGING_H

#include <ostream>

#include "TestCreator/Structs/ParsedResult.h"

class ParsingLogging {
 public:
  static void Log(std::ostream& Output, bool IsOkay,
                  const std::string& Message);
  static void OutputValue(std::ostream& Output, const std::string& Value);

  static void IncreaseIndentationLevel();
  static void DecreaseIndentationLevel();

  template <typename ReturnType, typename... ArgumentTypes>
  static ParsedResult<ReturnType> IndentForSubTask(
      std::ostream& Output, const std::string& Message,
      ParsedResult<ReturnType> (*Task)(
          ArgumentTypes&...),  // NOTE: THIS ISN'T A STD::FUNCTION BECAUSE THE
                               // TYPE DEDUCTION WILL GO MAD
      ArgumentTypes&... Args) {
    Log(Output, true, "Starting subtask " + Message);
    IncreaseIndentationLevel();
    ParsedResult<ReturnType> ToReturn = Task(Args...);
    DecreaseIndentationLevel();
    Log(Output, ToReturn.WasLegalInput, "Finished subtask " + Message);

    return ToReturn;
  }

  static void OutputBracketAcceptorStack(std::ostream& Output);

 private:
  static std::size_t IndentationLevel;
};

#endif /* PARSINGLOGGING_H */
