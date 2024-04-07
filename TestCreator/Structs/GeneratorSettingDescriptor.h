#ifndef TESTCREATOR_STRUCTS_GENERATORSETTINGDESCRIPTOR
#define TESTCREATOR_STRUCTS_GENERATORSETTINGDESCRIPTOR

#include <map>
#include <string>

typedef std::map<std::string, std::string> GeneratorSettingDescriptor;
/*
struct GeneratorSettingDescriptor {

  std::string Identifier;
  std::string Value;

  explicit GeneratorSettingDescriptor(std::string InIdentifier,
                                      std::string InValue)
      : Identifier(std::move(InIdentifier)), Value(std::move(InValue)) {}
}; */

#endif /* TESTCREATOR_STRUCTS_GENERATORSETTINGDESCRIPTOR */
