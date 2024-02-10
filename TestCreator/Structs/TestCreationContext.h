#ifndef TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H
#define TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H

#include <string>

#include "Filepath.h"
#include "GeneratorParameterStoreSeed.h"
#include "TestCreator/Consts/TestDefaults.h"

// POD struct
struct TestCreationContext {
  GeneratorParameterStoreSeed Params;
  Filepath& TestDefinitionPath;
  std::vector<std::string> AdditionalIncludes;
  std::string GeneratedFunctionName;
  std::size_t NumTestsToRun;
  std::string Category;
  int CurrentTestNumber;

  explicit TestCreationContext(Filepath& InTestDefinitionPath)
      : TestDefinitionPath(InTestDefinitionPath),
        Params(),
        AdditionalIncludes(),
        GeneratedFunctionName(),
        NumTestsToRun(DEFAULT_NUM_TESTS_TO_RUN),
        CurrentTestNumber(0) {}
};

#endif /* TESTCREATOR_STRUCTS_TESTCREATIONCONTEXT_H */
