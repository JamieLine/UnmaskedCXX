#ifndef VECTOROPERATIONS_H
#define VECTOROPERATIONS_H

#include <vector>

#include "TestCreator/Structs/ParsedResult.h"

auto AllOf(std::vector<bool> Bools) -> bool;

template <typename T>
auto AllLegal(std::vector<ParsedResult<T>> Results) -> bool;

template <typename T>
auto ExtractResults(std::vector<ParsedResult<T>> Results) -> std::vector<T>;

#endif /* VECTOROPERATIONS_H */
