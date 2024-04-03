#ifndef GENERATORS_GENERATORPARAMETERSTORE_H
#define GENERATORS_GENERATORPARAMETERSTORE_H

#include <iostream>
#include <map>
#include <queue>
#include <set>

#include "../Optional.h"
#include "../UserlandIncludes/UnmaskedTests.h"
#include "Logging.h"
#include "SetOperations.h"

class GeneratorParameterStore {
 public:
  GeneratorParameterStore() {}

  void ClearEverything();

  auto GetIntegerParameter(UnmaskedTestParameter Parameter) -> Optional<int>;
  auto GetFloatParameter(UnmaskedTestParameter Parameter) -> Optional<float>;

  template <typename T>
  void PushParameter(UnmaskedTestParameter Param, T Value);

  template <typename T>
  void PushTempParameter(UnmaskedTestParameter Param, T Value);

 private:
  std::map<UnmaskedTestParameter, Optional<int>> IntegerParameters;
  std::map<UnmaskedTestParameter, std::queue<int>> TemporaryIntegerParameters;

  std::map<UnmaskedTestParameter, Optional<float>> FloatParameters;
  std::map<UnmaskedTestParameter, std::queue<float>> TemporaryFloatParameters;
};

const std::set<UnmaskedTestParameter> LegalIntegerParameters = {
    INT_LOWER_BOUND,
    INT_UPPER_BOUND,
};

const std::set<UnmaskedTestParameter> LegalFloatParameters = {
    FLOAT_LOWER_BOUND,
    FLOAT_UPPER_BOUND,
};

// WARNING: WEAK TYPING AHEAD
template <typename T>
void GeneratorParameterStore::PushParameter(UnmaskedTestParameter Param,
                                            T Value) {
  if (SetContainsParameter(LegalIntegerParameters, Param)) {
    IntegerParameters[Param] = Optional<int>(Value, true);
  } else if (SetContainsParameter(LegalFloatParameters, Param)) {
    FloatParameters[Param] = Optional<float>(Value, true);
  } else {
    Log(std::cout, WARN,
        "Parameter was pushed to GeneratorParameterStore that it wasn't "
        "expecting");
    Log(std::cout, WARN, "Parameter was of type");
    Log(std::cout, VALUE_OUTPUT, std::to_string(Param));
  }
}

template <typename T>
void GeneratorParameterStore::PushTempParameter(UnmaskedTestParameter Param,
                                                T Value) {
  if (SetContainsParameter(LegalIntegerParameters, Param)) {
    TemporaryIntegerParameters[Param].emplace(int(Value));
  } else if (SetContainsParameter(LegalFloatParameters, Param)) {
    TemporaryFloatParameters[Param].emplace(float(Value));
  } else {
    Log(std::cout, WARN,
        "Temporary parameter was pushed to GeneratorParameterStore that it "
        "wasn't expecting");
    Log(std::cout, WARN, "Parameter was of type");
    Log(std::cout, VALUE_OUTPUT, std::to_string(Param));
  }
}

#endif /* GENERATORS_GENERATORPARAMETERSTORE_H */
