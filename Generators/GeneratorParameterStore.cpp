#include "GeneratorParameterStore.h"

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
    // In C++ versions before C++20, std::map has no methods to see if a key has a value
    // So here we'll use `try_emplace` as a dodgy workaround.
    // If the parameter had no value, we'll give it an empty queue.
    // Otherwise, it'll be left alone.
    // This effectively "lazy-loads" creation of the queue.
    // We'll do the permanent parameters.
    TemporaryIntegerParameters.try_emplace(Parameter);
    IntegerParameters.try_emplace(Parameter, Optional<int>(0, false));


    // We wish to pull a temporary value before a permanent one.
    if (!TemporaryIntegerParameters[Parameter].empty()) {
        // For some reason there's no public interface to do these
        // two steps in one line.
        auto ToReturn = TemporaryIntegerParameters[Parameter].front();
        TemporaryIntegerParameters[Parameter].pop();
        return Optional<int>(ToReturn, true);
    }

    // Now we find out if there really is a permanent parameter.
    else if (IntegerParameters[Parameter].DataExists) {
        return Optional<int>(IntegerParameters[Parameter].Data, true);
    }

    // Otherwise we simply signal back that there is no parameter available
    // in which case the caller should take a reasonable default.
    else {
        return Optional<int>(0, false);
    }
}
