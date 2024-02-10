#ifndef TESTCREATOR_STRUCTS_TESTSPECIFICATION_H
#define TESTCREATOR_STRUCTS_TESTSPECIFICATION_H

#include <string>
#include <utility>

#include "TestCreator/Structs/Filepath.h"

// A POD struct whch describes a test which has not been run yet.
// This is expected to be used by a function which writes tests into
// a file.
// E.g. bool Test_ThisFunction() { ... }
// NOT UnmaskedAlwaysReturnValueTest(...);
struct TestSpecification {
 public:
  Filepath Path;
  std::string Category;
  std::string Type;

  TestSpecification(Filepath InFilepath, std::string InCategory,
                    std::string InType)
      : Path(std::move(InFilepath)),
        Category(std::move(InCategory)),
        Type(std::move(InType)) {}
};

#endif /* TESTCREATOR_STRUCTS_TESTSPECIFICATION_H */
