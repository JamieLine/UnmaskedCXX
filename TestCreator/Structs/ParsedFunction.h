#ifndef TESTCREATOR_STRUCTS_PARSEDFUNCTION_H
#define TESTCREATOR_STRUCTS_PARSEDFUNCTION_H

#include <string>
#include <vector>

struct ParsedFunction {
 public:
  std::string Name;
  std::string ReturnType;
  std::vector<std::string> ArgumentTypes;

  ParsedFunction(const std::string& InName, const std::string& InReturnType,
                 const std::vector<std::string>& InArgumentTypes)
      : Name(InName),
        ReturnType(InReturnType),
        ArgumentTypes(InArgumentTypes) {}
};

#endif /* TESTCREATOR_STRUCTS_PARSEDFUNCTION_H */
