#include "CreateAdditionalIncludeLine.h"

#include <iostream>
#include <vector>

#include "Logging.h"
#include "StringOperations.h"
#include "TestCreator/Consts/TestMarkers.h"

auto CreateAdditionalIncludeLine(std::vector<std::string>::iterator& FirstToken)
    -> std::string {
  /* We expect an input as follows.
  1. FirstToken points to "UnmaskedAdditionalIncludeLine" at the beginning.
  2. The tokens that follow come from a string of the form
      `UnmaskedAdditionalIncludeLine("FILENAME");
  */

  // Begin by validating assumptions on the input
  if (*FirstToken != INCLUDE_FILE_MARKER) {
    Log(std::cout, LOG,
        "CreateAdditonalIncludeLine didn't get INCLUDE_FILE_MARKER as the "
        "first token");
    Log(std::cout, VALUE_OUTPUT, *FirstToken);
    Log(std::cout, VALUE_OUTPUT, INCLUDE_FILE_MARKER);
  } else {
    Log(std::cout, LOG, "CreateAdditionalIncludeLine got expected first token");
  }

  // Skip the marker, the open bracket, and the open quote
  FirstToken += 3;

  std::vector<std::string> IncludedFilepath;

  while (*FirstToken != "\"") {
    IncludedFilepath.push_back(*FirstToken);
    FirstToken++;
  }

  std::string Result =
      "#include \"" + JoinVectorOfStrings(IncludedFilepath, "") + "\"";

  return Result;
}