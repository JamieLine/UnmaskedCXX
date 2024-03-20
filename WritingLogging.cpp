#include "WritingLogging.h"

#include <iostream>

#include "Logging.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"

std::size_t WritingLogging::IndentationLevel = 0;
const int WidthOfIndent = 4;

void WritingLogging::Log(std::ostream& Output, bool IsOkay,
                         const std::string& Message) {
  Output << "<WRITER> ";
  if (!IsOkay) {
    Output << "!";

    int PaddingCharacterCount =
        (WidthOfIndent * WritingLogging::IndentationLevel) - 2;
    if (PaddingCharacterCount < 0) {
      PaddingCharacterCount = 0;
    }
    Output << std::string(PaddingCharacterCount, '-');

    Output << "!";
  } else {
    Output << std::string(WidthOfIndent * WritingLogging::IndentationLevel,
                          '-');
  }

  Output << Message;
  Output << std::endl;
}

void WritingLogging::OutputValue(std::ostream& Output,
                                 const std::string& Value) {
  Output << "<WRITER> ";
  Output << std::string(WidthOfIndent * WritingLogging::IndentationLevel, '-');
  Output << " <VALUE_OUTPUT> ";
  Output << Value;
  Output << std::endl;
}

void WritingLogging::IncreaseIndentationLevel() {
  WritingLogging::IndentationLevel++;
}

void WritingLogging::DecreaseIndentationLevel() {
  if (WritingLogging::IndentationLevel > 0) {
    WritingLogging::IndentationLevel--;
  }
}
