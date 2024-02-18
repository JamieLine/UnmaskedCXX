#include "VectorOperations.h"

#include <algorithm>

auto AllOf(std::vector<bool> Bools) -> bool {
  if (Bools.empty()) {
    return true;
  }

  return std::all_of(Bools.begin(), Bools.end(),
                     [](bool B) -> bool { return B; });
}

template <typename T>
auto AllLegal(std::vector<ParsedResult<T>> Results) -> bool {
  if (Results.empty()) {
    return true;
  }

  return std::all_of(Results.begin(), Results.end(),
                     [](ParsedResult<T> R) -> bool { return R.WasLegalInput; });
}

template <typename T>
auto ExtractResults(std::vector<ParsedResult<T>> Results) -> std::vector<T> {
  std::vector<T> ToReturn;
  ToReturn.reserve(Results.size());

  std::transform(Results.begin(), Results.end(), std::back_inserter(ToReturn),
                 [](ParsedResult<T> R) -> T { return R.Result; });

  return ToReturn;
}