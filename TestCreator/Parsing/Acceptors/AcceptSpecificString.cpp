#include "AcceptSpecificString.h"

#include <iostream>

#include "ParsingLogging.h"

auto AcceptSpecificString(TokenArray::iterator& FirstToken,
                          std::string TargetString) -> bool {
  bool ToReturn = (*FirstToken == TargetString);
  ParsingLogging::IncreaseIndentationLevel();
  ParsingLogging::Log(
      std::cout, ToReturn,
      "Looked for specific string. Looked for first, found second");
  ParsingLogging::OutputValue(std::cout, TargetString);
  ParsingLogging::OutputValue(std::cout, *FirstToken);
  ParsingLogging::DecreaseIndentationLevel();
  // Each parser or parser-fragment is expected to move the iterator it gets
  FirstToken++;

  return ToReturn;
}
