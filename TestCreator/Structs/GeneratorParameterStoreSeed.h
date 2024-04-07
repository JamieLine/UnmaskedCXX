#ifndef TESTCREATOR_STRUCTS_GENERATORPARAMETERSTORESEED_H
#define TESTCREATOR_STRUCTS_GENERATORPARAMETERSTORESEED_H

#include <string>
#include <vector>

struct GeneratorParameterStoreSeed {
 public:
  std::vector<std::pair<std::string, std::string>> ParametersAndValues;
  std::vector<std::pair<std::string, std::string>> TempParametersAndValues;

  GeneratorParameterStoreSeed();

  void ReadInParameterDeclaration(
      std::vector<std::string>::iterator& FirstToken);
  void ReadInTempParameterDeclaration(
      std::vector<std::string>::iterator& FirstToken);

  void ResetTempParameters();

  auto CreateGeneratorParameterStoreDefinition() const -> std::string;
};

#endif /* TESTCREATOR_STRUCTS_GENERATORPARAMETERSTORESEED_H */
