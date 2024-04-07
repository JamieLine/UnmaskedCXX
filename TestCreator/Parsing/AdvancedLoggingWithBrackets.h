#ifndef TESTCREATOR_PARSING_ADVANCEDLOGGINGWITHBRACKETS
#define TESTCREATOR_PARSING_ADVANCEDLOGGINGWITHBRACKETS

#include "AdvancedLogging.h"
#include "TestCreator/Structs/ParsedResult.h"

class AdvancedLoggingWithBrackets : public AdvancedLogging {
 public:
  AdvancedLoggingWithBrackets(std::string Title) : AdvancedLogging(Title) {}

  template <typename ReturnType, typename... ArgumentTypes>
  ParsedResult<ReturnType> IndentForSubTask(
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

  void OutputBracketAcceptorStack(std::ostream& Output);
};

static AdvancedLoggingWithBrackets ParsingLogging("PARSER");

#endif /* TESTCREATOR_PARSING_ADVANCEDLOGGINGWITHBRACKETS */
