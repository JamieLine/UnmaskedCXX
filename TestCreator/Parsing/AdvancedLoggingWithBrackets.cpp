#include "AdvancedLoggingWithBrackets.h"

#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"

void AdvancedLoggingWithBrackets::OutputBracketAcceptorStack(
    std::ostream& Output) {
  // The stack can only contain open brackets
  for (auto& Bracket : BracketAcceptor::CurrentlyOpenBrackets) {
    Output << BracketAcceptor::GetOpeningBrackets(Bracket);
    Output << " ";
  }

  Output << std::endl;
}
