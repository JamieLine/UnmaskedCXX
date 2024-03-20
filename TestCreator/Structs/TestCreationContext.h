#ifndef TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H
#define TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H

#include <string>

#include "Filepath.h"
#include "GeneratorParameterStoreSeed.h"
#include "TestCreator/Consts/TestDefaults.h"

// POD struct
struct TestCreationContext {
  GeneratorParameterStoreSeed Params;
  Filepath TestDefinitionPath;
  std::vector<std::string> AdditionalIncludes;
  std::string Category;
  // TODO(linej): Maybe this should have an empty checker?
  // Other fields make sense when empty but these three don't.
  std::string GeneratedFunctionName;
  std::size_t NumTestsToRun;
  int CurrentTestNumber;

  // TODO: THIS IS IN THE WRONG PLACE
  // THIS STRUCT TALKS ABOUT THE CONTEXT THE TEST WE ARE CREATING WAS DEFINED IN
  // WHEREAS THIS VARIABLE IS TALKING ABOUT THE CONTEXT OF SPECIFIC TEXT EDITS
  // WITHIN A GENERATED TEST
  std::size_t CurrentIndentationLevel;

  explicit TestCreationContext(const Filepath& InTestDefinitionPath)
      : TestDefinitionPath(InTestDefinitionPath),
        Params(),
        AdditionalIncludes(),
        GeneratedFunctionName(),
        NumTestsToRun(DEFAULT_NUM_TESTS_TO_RUN),
        CurrentTestNumber(0),
        CurrentIndentationLevel(0) {}
};

#endif /* TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H */
