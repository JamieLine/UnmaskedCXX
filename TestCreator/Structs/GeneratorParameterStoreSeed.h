#pragma once

#include <vector>
#include <queue>
#include <string>

struct GeneratorParameterStoreSeed {
public:
    std::vector<std::pair<std::string, std::string>> ParametersAndValues;
    std::queue<std::pair<std::string, std::string>> TempParametersAndValues;

    GeneratorParameterStoreSeed();

    void ReadInParameterDeclaration(std::vector<std::string>::iterator& FirstToken);
    void ReadInTempParameterDeclaration(std::vector<std::string>::iterator& FirstToken);

    void ResetTempParameters();
};
