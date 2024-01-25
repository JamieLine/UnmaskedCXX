#pragma once

#include "../UserlandIncludes/UnmaskedTests.h"
#include "../Optional.h"
#include <map>
#include <queue>

class GeneratorParameterStore {
public:
    GeneratorParameterStore() {}

    void ClearEverything();

    Optional<int> GetIntegerParameter(UnmaskedTestParameter Parameter);

private:
    std::map<UnmaskedTestParameter, Optional<int>> IntegerParameters;
    std::map<UnmaskedTestParameter, std::queue<int>> TemporaryIntegerParameters;
};