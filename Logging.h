#ifndef LOGGING_H
#define LOGGING_H

#include <functional>
#include <ostream>

#include "TestCreator/Structs/ParsedResult.h"

const std::string LOG = "LOG";
const std::string WARN = "WARN";
const std::string ERROR = "ERROR";
const std::string VALUE_OUTPUT = "VALUE_OUTPUT";

void Log(std::ostream& Output, const std::string& Level,
         const std::string& Message);

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
      std::function<ParsedResult<ReturnType>(ArgumentTypes...)> Task,
      ArgumentTypes... Args) {
    Log(Output, true, "Starting subtask " + Message);
    IncreaseIndentationLevel();
    ParsedResult<ReturnType> ToReturn = Task(Args...);
    DecreaseIndentationLevel();
    Log(Output, ToReturn.WasLegalInput, "Finished subtask " + Message);

    return ToReturn;
  }

 private:
  static std::size_t IndentationLevel;
};

#endif /* LOGGING_H */
