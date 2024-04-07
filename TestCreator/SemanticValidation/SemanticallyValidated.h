#ifndef TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H
#define TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H

#include "SemanticValidator.h"

template <typename T>
struct SemanticallyValidated {
 public:
  T Object;

  friend SemanticValidator;

 private:
  SemanticallyValidated<T>(T InObject) : Object(InObject) {}
};

#endif /* TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H */
