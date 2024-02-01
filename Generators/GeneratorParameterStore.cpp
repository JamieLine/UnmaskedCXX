#include "GeneratorParameterStore.h"
#include "MapOperations.h"
#include "Logging.h"
#include <iostream>
#include <set>
#include "SetOperations.h"

void GeneratorParameterStore::ClearEverything()
{
    for (auto& ParameterValuePair : IntegerParameters) {
        // Declare that there is no data.
        // Actually deleting the data is both
        // meaningless and unnecessary. 
        ParameterValuePair.second.DataExists = false;
    }

    for (auto& ParameterValuePair : TemporaryIntegerParameters) {
        // Here we actually empty out the data
        // std::queue has no "empty this queue"
        // method, so we replace it with an empty
        // one.
        ParameterValuePair.second = std::queue<int>();
    }
}

Optional<int> GeneratorParameterStore::GetIntegerParameter(UnmaskedTestParameter Parameter)
{
    // We wish to pull a temporary value before a permanent one.
    // Check that the map contains the key, then see if that key leads to a useful int
    if (TemporaryIntegerParameters.find(Parameter) != TemporaryIntegerParameters.end()) {
        if (!TemporaryIntegerParameters[Parameter].empty()) {
            // For some reason there's no public interface to do these
            // two steps in one line.
            auto ToReturn = TemporaryIntegerParameters[Parameter].front();
            TemporaryIntegerParameters[Parameter].pop();
            Log(std::cout, LOG, "Fetched Temporary Parameter");
            return Optional<int>(ToReturn, true);
        }
    }
    
    if (IntegerParameters.find(Parameter) != IntegerParameters.end()) {
        // Now we find out if there really is a permanent parameter.
        if (IntegerParameters[Parameter].DataExists) {
            Log(std::cout, LOG, "Fetched Normal Parameter");
            return IntegerParameters[Parameter];
        }
    }

    // Otherwise we simply signal back that there is no parameter available
    // in which case the caller should take a reasonable default.
    return Optional<int>(0, false);
}

