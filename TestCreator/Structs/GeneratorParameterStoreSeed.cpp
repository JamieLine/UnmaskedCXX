#include "GeneratorParameterStoreSeed.h"
#include "UserlandIncludes/UnmaskedTests.h"

#include <set>
#include "SetOperations.h"

//std::set<UnmaskedTestParameter> IntegerParameterKeys = {
    //INT_LOWER_BOUND,
    //INT_UPPER_BOUND,
//};

GeneratorParameterStoreSeed::GeneratorParameterStoreSeed() {}

// TODO: Should this and the temp version be merged and have an input arg which specifies
// temporary-ness?
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

    TempParametersAndValues.emplace_back(Param, Value);
}

void GeneratorParameterStoreSeed::ResetTempParameters()
{
    TempParametersAndValues = std::vector<std::pair<std::string, std::string>>();
}

std::string GeneratorParameterStoreSeed::CreateGeneratorParameterStoreDefinition() {
    std::string ToReturn = "";
    for (auto ParameterAndValue : ParametersAndValues) {
        // Dealing with the real types of these values is the job of the Generators
        // that pull the values out.

        ToReturn += "\t\tParameters.PushParameter(" + ParameterAndValue.first + "," + ParameterAndValue.second +");\n";
    }

    for (auto TempParameterAndValue : TempParametersAndValues) {
        ToReturn += "\t\tParameters.PushTempParameter(" + TempParameterAndValue.first + "," + TempParameterAndValue.second +");\n";
    }

    return ToReturn;
}