#include "Logging.h"

void Log(std::ostream& Output, const std::string& Level,
         const std::string& Message) {
  Output << "<";
  Output << Level;
  Output << ">";
  Output << " ";
  Output << Message;
  Output << std::endl;
}

std::size_t ParsingLogging::IndentationLevel = 0;
const int WidthOfIndent = 4;

void ParsingLogging::Log(std::ostream& Output, bool IsOkay,
                         const std::string& Message) {
  Output << "<PARSER> ";
  if (!IsOkay) {
    Output << "!";
    Output << std::string('-',
                          WidthOfIndent * ParsingLogging::IndentationLevel - 2);

    Output << "!";
  } else {
    Output << std::string('-',
                          WidthOfIndent * ParsingLogging::IndentationLevel);
  }

  Output << Message;
  Output << std::endl;
}

void ParsingLogging::OutputValue(std::ostream& Output,
                                 const std::string& Value) {
  Output << "<PARSER> ";
  Output << std::string('-', WidthOfIndent * ParsingLogging::IndentationLevel);
  Output << " <VALUE_OUTPUT> ";
  Output << Value;
  Output << std::endl;
}

void ParsingLogging::IncreaseIndentationLevel() {
  ParsingLogging::IndentationLevel++;
}

void ParsingLogging::DecreaseIndentationLevel() {
  if (ParsingLogging::IndentationLevel > 0) {
    ParsingLogging::IndentationLevel--;
  }
}
