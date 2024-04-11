#ifndef TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H
#define TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H

// Forward declaration to remove a recursion issue class SemanticValidator;
class SemanticValidator;

template <typename T>
struct SemanticallyValidated {
 public:
  const T Object;

  friend SemanticValidator;

 private:
  SemanticallyValidated(T InObject) : Object(InObject) {}
};

#endif /* TESTCREATOR_SEMANTICVALIDATON_SEMANTICALLYVALIDATED_H */
