#ifndef TESTCREATOR_WRITING_REPLACESYMBOLANDLOG_H
#define TESTCREATOR_WRITING_REPLACESYMBOLANDLOG_H

#include <iostream>
#include <ostream>

#include "AdvancedLogging.h"
#include "StringOperations.h"

inline std::string ReplaceSymbolAndLog(std::ostream& Output,
                                       std::string ToReturn,
                                       const std::string& ToReplace,
                                       const std::string& Replacement) {
  WritingLogging.Log(std::cout, true, "Replacing " + ToReplace);
  WritingLogging.OutputValue(std::cout, Replacement);
  ToReturn = ReplaceAllInString(ToReturn, ToReplace, Replacement);
  return ToReturn;
}

#endif /* TESTCREATOR_WRITING_REPLACESYMBOLANDLOG_H */
