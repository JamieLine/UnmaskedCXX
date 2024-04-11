#ifndef TESTCREATOR_SEMANTICVALIDATION_SEMANTICVALIDATOR_H
#define TESTCREATOR_SEMANTICVALIDATION_SEMANTICVALIDATOR_H

#include "Optional.h"
#include "SemanticValidation/SemanticallyValidated.h"
#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"

class SemanticValidator {
 public:
  static Optional<SemanticallyValidated<GeneratorSettingBunch>> Validate(
      GeneratorSettingBunch Bunch);

  static Optional<SemanticallyValidated<GeneratorSettingDescriptor>> Validate(
      GeneratorSettingDescriptor Descriptor);

  static Optional<SemanticallyValidated<ParsedUnmaskedPredicateTest>> Validate(
      ParsedUnmaskedPredicateTest Test);
};

#endif /* TESTCREATOR_SEMANTICVALIDATION_SEMANTICVALIDATOR_H */
