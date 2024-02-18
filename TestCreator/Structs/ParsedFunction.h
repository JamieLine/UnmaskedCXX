#ifndef TESTCREATOR_STRUCTS_PARSEDFUNCTION_H
#define TESTCREATOR_STRUCTS_PARSEDFUNCTION_H

#include <string>
#include <vector>

struct ParsedFunction {
 public:
  std::string Name;
  std::string ReturnType;
  std::vector<std::string> ArgumentTypes;

  ParsedFunction() = default;
};

#endif /* TESTCREATOR_STRUCTS_PARSEDFUNCTION_H */
