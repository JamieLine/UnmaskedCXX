
// Include this file anywhere you wish to specify UnmaskedCXX tests.
// The specifications will never be run directly, they are simply there
// so UnmaskedCXX knows where to look.

#ifndef USERLANDINCLUDES_UNMASKEDTESTS_H
#define USERLANDINCLUDES_UNMASKEDTESTS_H

#include <functional>
#include <string>
#include <vector>

struct GeneratorSettings {
 public:
  int Index;
  bool Fixed;
  std::string Value;

  // To specify a generator which isn't Generator_{ArgumentType}, set this to
  // something other than ""
  std::string GeneratorType;

  std::vector<std::pair<std::string, std::string>> Parameters;

  // To specify the exact code used to generate this value manually, use this.
  // This must contain a lambda function which returns the required value, and
  // is allowed to capture pregenerated values for every index, including ones
  // which will be later overriden.
  std::string GeneratorScript;
};

enum UnmaskedSeverity {
  LIGHT,
  MEDIUM,
  SERIOUS,
};

enum UnmaskedTestParameter {
  INT_LOWER_BOUND,
  INT_UPPER_BOUND,
  FLOAT_LOWER_BOUND,
  FLOAT_UPPER_BOUND,
};

template <typename ReturnType, typename... TestedArgumentTypes>
void UnmaskedStabilisingSetTest(
    std::function<ReturnType(TestedArgumentTypes...)> TestedFunction,
    const std::vector<GeneratorSettings>& SuppliedSettings) {
  // Intentionally left empty.
}

template <typename ReturnType, typename... TestedArgumentTypes>
void UnmaskedAlwaysReturnsValueTest(
    std::function<ReturnType(TestedArgumentTypes...)> TestedFunction,
    ReturnType FixedValue,
    const std::vector<GeneratorSettings>& SuppliedSettings) {
  // Intentionally left empty.
}

template <typename... Ts>
void UnmaskedSetParameter(UnmaskedTestParameter /*unused*/, Ts... /*unused*/) {
  // Intentionally left empty.
}

template <typename... Ts>
void UnmaskedIncludeFile(Ts... /*unused*/) {
  // Intentionally left empty.
}

inline void UnmaskedSetCategory(const std::string& /*unused*/) {
  // Intentionally left empty.
}

inline void UnmaskedSetSeverity(UnmaskedSeverity /* unused */) {
  // Intentionally left empty.
}

template <typename ReturnType, typename... TestedArgumentTypes,
          typename PredicateType>
inline void UnmaskedPredicateTest(
    std::function<ReturnType(TestedArgumentTypes...)> TestedFunction,
    PredicateType Predicate,
    const std::vector<GeneratorSettings>& SuppliedSettings) {
  // Intentionally left empty.
}

#endif /* USERLANDINCLUDES_UNMASKEDTESTS_H */
