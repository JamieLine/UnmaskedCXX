#ifndef TESTCREATOR_STRUCTS_ARGUMENTTYPE_H
#define TESTCREATOR_STRUCTS_ARGUMENTTYPE_H

#include <string>
#include <utility>

// POD struct which holds the type of an argument and a few small utility
// methods
struct ArgumentType {
 public:
  std::string Type;

  explicit ArgumentType(std::string InType) : Type(std::move(InType)) {}

  auto GetGeneratorType() const -> std::string { return "Generator_" + Type; }
  auto GetGeneratorCall() const -> std::string {
    return GetGeneratorType() + "()";
  }

  explicit operator std::string() const { return Type; }
};

#endif /* TESTCREATOR_STRUCTS_ARGUMENTTYPE_H */
