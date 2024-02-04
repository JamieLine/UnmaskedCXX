#ifndef TESTCREATOR_STRUCTS_ARGUMENTVALUE_H
#define TESTCREATOR_STRUCTS_ARGUMENTVALUE_H

#include <string>
#include <utility>

// POD struct, used to leverage strong typing as a form of error checking
// against badly ordered arguments to functions.
struct ArgumentValue {
 public:
  std::string Value;

  explicit ArgumentValue(std::string InValue) : Value(std::move(InValue)) {}
  explicit operator std::string() const { return Value; }
};

#endif /* TESTCREATOR_STRUCTS_ARGUMENTINDEX_H */
