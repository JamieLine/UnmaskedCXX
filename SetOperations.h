#ifndef SETOPERATIONS_H
#define SETOPERATIONS_H

#include <set>

#include "UserlandIncludes/UnmaskedTests.h"

template <typename T, typename... Ts>
auto SetContainsItem(std::set<T, Ts...>& Set, T Item) -> bool;

template <typename T, typename... Ts>
auto SetContainsItem(const std::set<T, Ts...>& ConstSet, T Item) -> bool;

auto SetContainsParameter(const std::set<UnmaskedTestParameter>& ParamSet,
                          UnmaskedTestParameter Item) -> bool;

#endif /* SETOPERATIONS_H */
