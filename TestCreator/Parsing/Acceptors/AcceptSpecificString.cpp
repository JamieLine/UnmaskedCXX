#include "AcceptSpecificString.h"

auto AcceptSpecificString(TokenArray::iterator& FirstToken,
                          std::string TargetString) -> bool {
  bool ToReturn = (*FirstToken == TargetString);
  // Each parser or parser-fragment is expected to move the iterator it gets
  FirstToken++;

  return ToReturn;
}