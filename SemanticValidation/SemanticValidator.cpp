
#include "SemanticValidation/SemanticValidator.h"

#include <iostream>

#include "AdvancedLogging.h"
#include "MapOperations.h"
#include "Optional.h"
#include "SemanticValidation/SemanticallyValidated.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/GeneratorSettingDescriptor.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"

Optional<SemanticallyValidated<GeneratorSettingBunch>>
SemanticValidator::Validate(GeneratorSettingBunch Bunch) {
  SemanticLogging.Log(std::cout, true,
                      "Beginning to check GeneratorSettingBunch");
  SemanticLogging.IncreaseIndentationLevel();

  bool ToReturn = true;
  for (const auto& Descriptor : Bunch.Settings) {
    if (!Validate(Descriptor).DataExists) {
      ToReturn = false;
    }
  }
  SemanticLogging.DecreaseIndentationLevel();
  SemanticLogging.Log(std::cout, ToReturn,
                      "Finished checking GeneratorSettingBunch");

  // Every check passed

  return Optional<SemanticallyValidated<GeneratorSettingBunch>>(Bunch,
                                                                ToReturn);
}

Optional<SemanticallyValidated<GeneratorSettingDescriptor>>
SemanticValidator::Validate(GeneratorSettingDescriptor Descriptor) {
  if (!MapContainsKey(Descriptor, std::string("Index"))) {
    // Every bunch must contain an Index

    // TODO: This should somehow be ready to produce Good Error Messages
    SemanticLogging.Log(std::cout, false,
                        "GeneratorSettingDescriptor had no index");

    return Optional<SemanticallyValidated<GeneratorSettingDescriptor>>(
        Descriptor, false);
  }

  SemanticLogging.Log(std::cout, true, "GeneratorSettingDescriptor was legal.");
  return Optional<SemanticallyValidated<GeneratorSettingDescriptor>>(Descriptor,
                                                                     true);
}

Optional<SemanticallyValidated<ParsedUnmaskedPredicateTest>>
SemanticValidator::Validate(ParsedUnmaskedPredicateTest Test) {
  // TODO: Maybe Validate the other fields

  SemanticLogging.Log(std::cout, true, "Checking ParsedUnmaskedPredicateTest");
  SemanticLogging.IncreaseIndentationLevel();
  bool ToReturn = Validate(Test.GeneratorSettings).DataExists;
  SemanticLogging.DecreaseIndentationLevel();
  SemanticLogging.Log(std::cout, ToReturn,
                      "Finished checking ParsedUnmaskedPredicateTest");
  return Optional<SemanticallyValidated<ParsedUnmaskedPredicateTest>>(Test,
                                                                      ToReturn);
}
