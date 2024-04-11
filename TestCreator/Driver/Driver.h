#ifndef TESTCREATOR_DRIVER
#define TESTCREATOR_DRIVER

#include "SemanticValidation/SemanticallyValidated.h"
#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TestCreationContext.h"
#include "TestCreator/Structs/TestCreationStatus.h"

class Driver {
 public:
  static auto ParseInputFile(const Filepath& FileAddress) -> TestCreationStatus;
  static auto WriteAllStoredInputs() -> TestCreationStatus;

 private:
  static std::vector<Filepath> GeneratedSourceFilepaths;

  static auto WriteMainDriverProgram() -> TestCreationStatus;

  static std::vector<std::pair<
      TestCreationContext, SemanticallyValidated<ParsedUnmaskedPredicateTest>>>
      StoredPredicateTests;

  // A stabilising set test is a predicate test with a pre-written predicate.
  static std::vector<std::pair<
      TestCreationContext, SemanticallyValidated<ParsedUnmaskedPredicateTest>>>
      StoredStabilisingSetTests;

  static std::vector<std::pair<
      TestCreationContext, SemanticallyValidated<ParsedUnmaskedPredicateTest>>>
      StoredAlwaysReturnValueTests;
};

#endif /* TESTCREATOR_DRIVER */
