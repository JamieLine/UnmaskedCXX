#ifndef TESTCREATOR_STRUCTS_LEGALIDENTIFIER_H
#define TESTCREATOR_STRUCTS_LEGALIDENTIFIER_H

#include <string>
#include <utility>

// POD struct, used to leverage strong typing as a form of error checking
// against badly ordered arguments to functions.
struct LegalIdentifier {
 public:
  std::string ID;

  explicit LegalIdentifier(std::string InID) : ID(std::move(InID)) {}
  explicit operator std::string() { return ID; }
};

#endif /* TESTCREATOR_STRUCTS_LEGALIDENTIFIER_H */
