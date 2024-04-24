#include "AcceptCastToType.h"
#include "TestCreator/Parsing/Acceptors/AcceptSpecificString.h"
#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"

#include "TestCreator/Structs/Task.h"

auto AcceptCastToType(TokenArray::RawTokenIterator& FirstToken, std::string TypeName)
    -> bool {
  // Don't need to subtask these as we can give a very good error message from here.
  bool OpeningBracketWasLegal = BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  if (!OpeningBracketWasLegal) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Could not find opening bracket. Expected '(', and found " + PreviousToken);
    return false;
  }

  bool TypeNameWasLegal = AcceptSpecificString(FirstToken, TypeName);
  if (!TypeNameWasLegal) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Found incorrect type name. Expected '" + TypeName + "', and found " + PreviousToken);
    return false;
  }

  // We don't need to subtask this, as we already know the top of the BracketAcceptor stack is an opening rounded bracket.
  bool ClosingBracketWasLegal = BracketAcceptor::AcceptOpeningBracket(FirstToken, ROUNDED);
  if (!ClosingBracketWasLegal) {
    std::string PreviousToken = *(FirstToken - 1);
    FirstToken.GetCurrentTask()->PushErrorMessage("Could not find closing bracket. Expected ')', and found " + PreviousToken);
    return false;
  }

  // To get here, every part must have been legal.
  return true;
  
}