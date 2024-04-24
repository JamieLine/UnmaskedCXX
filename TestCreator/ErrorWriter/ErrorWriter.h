#ifndef TESTCREATOR_ERRORWRITER_ERRORWRITER_H
#define TESTCREATOR_ERRORWRITER_ERRORWRITER_H

#include "TestCreator/Structs/TokenArray.h"
class ErrorWriter {
  TokenArray& ThisArray;

 public:
  ErrorWriter(TokenArray& InArray) : ThisArray(InArray) {}

  void PrintEverything();
};

// TODO: Litter the rest of the code with `PerformSubTask` calls
// TODO: Perform some actually useful processing to determine what tasks caused
// errors
// TODO: Turn that whole ensemble into a useful error message.

#endif /* TESTCREATOR_ERRORWRITER_ERRORWRITER_H */
