#include "AdvancedLogging.h"

#include <iostream>

#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"

const int WidthOfIndent = 4;

void AdvancedLogging::Log(std::ostream& Output, bool IsOkay,
                          const std::string& Message) {
  Output << "<PARSER> ";
  if (!IsOkay) {
    Output << "!";

    int PaddingCharacterCount = (WidthOfIndent * IndentationLevel) - 2;
    if (PaddingCharacterCount < 0) {
      PaddingCharacterCount = 0;
    }
    Output << std::string(PaddingCharacterCount, '-');

    Output << "!";
  } else {
    Output << std::string(WidthOfIndent * IndentationLevel, '-');
  }

  Output << Message;
  Output << std::endl;
}

void AdvancedLogging::OutputValue(std::ostream& Output,
                                  const std::string& Value) {
  Output << "<PARSER> ";
  Output << std::string(WidthOfIndent * IndentationLevel, '-');
  Output << " <VALUE_OUTPUT> ";
  Output << Value;
  Output << std::endl;
}

void AdvancedLogging::IncreaseIndentationLevel() { IndentationLevel++; }

void AdvancedLogging::DecreaseIndentationLevel() {
  if (IndentationLevel > 0) {
    IndentationLevel--;
  }
}
