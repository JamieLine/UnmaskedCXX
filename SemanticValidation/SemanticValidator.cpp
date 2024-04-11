
#include "SemanticValidation/SemanticValidator.h"

#include <iostream>

#include "AdvancedLogging.h"
#include "MapOperations.h"
#include "Optional.h"
#include "SemanticValidation/SemanticallyValidated.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/GeneratorSettingDescriptor.h"

// TODO: Write Validate(UnmaskedPredicateTest) and get the driver to run it

Optional<SemanticallyValidated<GeneratorSettingBunch>>
SemanticValidator::Validate(GeneratorSettingBunch Bunch) {
  SemanticLogging.Log(std::cout, true,
                      "Beginning to check GeneratorSettingBunch");
  SemanticLogging.IncreaseIndentationLevel();

  bool ToReturn;
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

    // TODO: A) This entire loop should swap to a new function
    // `Validate(GeneratorSettingDescriptor)`
    // TODO: B) This should somehow be ready to produce Good Error Messages
    SemanticLogging.Log(std::cout, false,
                        "GeneratorSettingDescriptor had no index");

    return Optional<SemanticallyValidated<GeneratorSettingDescriptor>>(
        Descriptor, false);
  }

  SemanticLogging.Log(std::cout, false,
                      "GeneratorSettingDescriptor was legal.");
  return Optional<SemanticallyValidated<GeneratorSettingDescriptor>>(Descriptor,
                                                                     true);
}
