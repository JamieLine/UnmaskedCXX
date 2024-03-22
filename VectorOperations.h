#ifndef VECTOROPERATIONS_H
#define VECTOROPERATIONS_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "ParsingLogging.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

inline auto PrintAround(const TokenArray::iterator& Token,
                        const TokenArray& Array, std::size_t Range) -> void {
  TokenArray::iterator Start = Token;

  int StartDecrements = 0;

  while (StartDecrements < Range && Start != Array.begin()) {
    Start--;
    StartDecrements++;
  }

  TokenArray::iterator End = Token;

  int EndIncrements = 0;

  while (EndIncrements < Range && End != Array.end()) {
    End++;
    EndIncrements++;
  }

  TokenArray PrintedTokens = TokenArray(Start, End);

  for (auto ThisToken : PrintedTokens) {
    ParsingLogging::OutputValue(std::cout, ThisToken);
  }
}

inline auto PrintAround(const TokenArray::iterator& Token,
                        const TokenArray& Array) -> void {
  const std::size_t DEFAULT_RANGE = 25;
  PrintAround(Token, Array, DEFAULT_RANGE);
}

template <typename T>
inline auto PrintVector(std::ostream& Output, const std::vector<T>& Vec)
    -> void {
  for (T Item : Vec) {
    Output << Item;
    Output << ",";
  }

  Output << std::endl;
}

inline auto AllOf(std::vector<bool> Bools) -> bool {
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

#endif /* VECTOROPERATIONS_H */