#ifndef TESTCREATOR_STRUCTS_TESTRESULT_H
#define TESTCREATOR_STRUCTS_TESTRESULT_H

#include <string>
#include <utility>

// A POD struct which groups together data regarding a test which has already
// been completed. Used in TestRunnerTemplate and generated runners.
struct TestResult {
 public:
  std::string Name;
  std::string Type;

  bool Passed;

  TestResult(std::string InName, std::string InType, bool InPassed)
      : Name(std::move(InName)), Type(std::move(InType)), Passed(InPassed) {}
};

#endif /* TESTCREATOR_STRUCTS_TESTRESULT_H */
