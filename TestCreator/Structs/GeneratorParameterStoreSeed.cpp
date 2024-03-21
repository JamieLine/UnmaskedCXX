#include "GeneratorParameterStoreSeed.h"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <set>

#include "SetOperations.h"
#include "UserlandIncludes/UnmaskedTests.h"

// std::set<UnmaskedTestParameter> IntegerParameterKeys = {
// INT_LOWER_BOUND,
// INT_UPPER_BOUND,
//};

GeneratorParameterStoreSeed::GeneratorParameterStoreSeed() = default;

// TODO(linej): Should this and the temp version be merged and have an input arg
// which specifies temporary-ness?
void GeneratorParameterStoreSeed::ReadInParameterDeclaration(
    std::vector<std::string>::iterator& FirstToken) {
  auto& CurrentToken = FirstToken;

  // Skip the marker and the opening bracket
  CurrentToken += 2;
  std::string Param = *CurrentToken;

  // Skip the comma between the parameter and its value
  CurrentToken += 2;
  std::string Value = *CurrentToken;

  ParametersAndValues.emplace_back(Param, Value);
}

void GeneratorParameterStoreSeed::ReadInTempParameterDeclaration(
    std::vector<std::string>::iterator& FirstToken) {
  auto& CurrentToken = FirstToken;

  // Skip the marker and the opening bracket
  CurrentToken += 2;
  std::string Param = *CurrentToken;

  // Skip the comma between the parameter and its value
  CurrentToken += 2;
  std::string Value = *CurrentToken;

  TempParametersAndValues.emplace_back(Param, Value);
}

void GeneratorParameterStoreSeed::ResetTempParameters() {
  TempParametersAndValues = std::vector<std::pair<std::string, std::string>>();
}

auto GeneratorParameterStoreSeed::CreateGeneratorParameterStoreDefinition()
    const -> std::string {
  // Cppcheck tells us to std::accumulate here.
  // In C++20 this is fine, but in C++11 its extremely slow
  // Because accumulate doesn't acknowledge std::move exists.

  std::string ToReturn;
  for (const auto& ParameterAndValue : ParametersAndValues) {
    // Dealing with the real types of these values is the job of the Generators
    // that pull the values out.

    // cppcheck-suppress useStlAlgorithm
    ToReturn += "\t\tParameters.PushParameter(" + ParameterAndValue.first +
                "," + ParameterAndValue.second + ");\n";
  }

  for (const auto& TempParameterAndValue : TempParametersAndValues) {
    // cppcheck-suppress useStlAlgorithm
    ToReturn += "\t\tParameters.PushTempParameter(" +
                TempParameterAndValue.first + "," +
                TempParameterAndValue.second + ");\n";
  }

  return ToReturn;
}
