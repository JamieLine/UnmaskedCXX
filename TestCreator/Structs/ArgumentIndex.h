#ifndef TESTCREATOR_STRUCTS_ARGUMENTINDEX_H
#define TESTCREATOR_STRUCTS_ARGUMENTINDEX_H

#include <string>
#include <utility>

// POD struct, used to leverage strong typing as a form of error checking
// against badly ordered arguments to functions.
struct ArgumentIndex {
 public:
  std::string Index;

  explicit ArgumentIndex(std::string InIndex) : Index(std::move(InIndex)) {}
  explicit operator std::string() const { return Index; }
};

#endif /* TESTCREATOR_STRUCTS_ARGUMENTINDEX_H */
