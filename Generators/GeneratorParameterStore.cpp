#include "GeneratorParameterStore.h"

#include <iostream>
#include <set>

#include "Logging.h"
#include "MapOperations.h"
#include "SetOperations.h"

void GeneratorParameterStore::ClearEverything() {
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

auto GeneratorParameterStore::GetIntegerParameter(
    UnmaskedTestParameter Parameter) -> Optional<int> {
  // We wish to pull a temporary value before a permanent one.
  // Check that the map contains the key, then see if that key leads to a useful
  // int
  if (TemporaryIntegerParameters.find(Parameter) !=
      TemporaryIntegerParameters.end()) {
    if (!TemporaryIntegerParameters[Parameter].empty()) {
      // For some reason there's no public interface to do these
      // two steps in one line.
      auto ToReturn = TemporaryIntegerParameters[Parameter].front();
      TemporaryIntegerParameters[Parameter].pop();
      Log(std::cout, LOG, "Fetched Temporary Parameter");
      return {ToReturn, true};
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
  return {0, false};
}

auto GeneratorParameterStore::GetFloatParameter(UnmaskedTestParameter Parameter)
    -> Optional<float> {
  // We wish to pull a temporary value before a permanent one.
  // Check that the map contains the key, then see if that key leads to a useful
  // int
  if (TemporaryFloatParameters.find(Parameter) !=
      TemporaryFloatParameters.end()) {
    if (!TemporaryFloatParameters[Parameter].empty()) {
      // For some reason there's no public interface to do these
      // two steps in one line.
      auto ToReturn = TemporaryFloatParameters[Parameter].front();
      TemporaryFloatParameters[Parameter].pop();
      Log(std::cout, LOG, "Fetched Temporary Parameter");
      return {ToReturn, true};
    }
  }

  if (FloatParameters.find(Parameter) != FloatParameters.end()) {
    // Now we find out if there really is a permanent parameter.
    if (FloatParameters[Parameter].DataExists) {
      Log(std::cout, LOG, "Fetched Normal Parameter");
      return FloatParameters[Parameter];
    }
  }

  // Otherwise we simply signal back that there is no parameter available
  // in which case the caller should take a reasonable default.
  return {0.0f, false};
}