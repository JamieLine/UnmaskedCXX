#include "GeneratorParameterStoreSeed.h"

GeneratorParameterStoreSeed::GeneratorParameterStoreSeed() {}

void GeneratorParameterStoreSeed::ReadInParameterDeclaration(std::vector<std::string>::iterator &FirstToken)
{
    auto& CurrentToken = FirstToken;

    // Skip the marker and the opening bracket
    CurrentToken += 2;
    std::string Param = *CurrentToken;

    // Skip the comma between the parameter and its value
    CurrentToken += 2;
    std::string Value = *CurrentToken;

    ParametersAndValues.emplace_back(Param, Value);
}

void GeneratorParameterStoreSeed::ReadInTempParameterDeclaration(std::vector<std::string>::iterator &FirstToken)
{
    auto& CurrentToken = FirstToken;

    // Skip the marker and the opening bracket
    CurrentToken += 2;
    std::string Param = *CurrentToken;

    // Skip the comma between the parameter and its value
    CurrentToken += 2;
    std::string Value = *CurrentToken;

    TempParametersAndValues.emplace(Param, Value);
}

void GeneratorParameterStoreSeed::ResetTempParameters()
{
    TempParametersAndValues = std::queue<std::pair<std::string, std::string>>();
}
