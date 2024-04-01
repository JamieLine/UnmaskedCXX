#ifndef TESTINCLUDES_BOOLTOSTRING_H
#define TESTINCLUDES_BOOLTOSTRING_H

#include <string>

inline std::string BoolToString(bool B) {
  if (B) {
    return "True";
  }
  return "False";
}

#endif /* TESTINCLUDES_BOOLTOSTRING_H */
