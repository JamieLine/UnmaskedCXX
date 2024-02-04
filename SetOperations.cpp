#include "SetOperations.h"

#include <set>

#include "UnmaskedTests.h"

template <typename T, typename... Ts>
auto SetContainsItem(std::set<T, Ts...>& Set, T Item) -> bool {
  return (Set.find(Item) != Set.end());
}

template <typename T, typename... Ts>
auto SetContainsItem(const std::set<T, Ts...>& Set, T Item) -> bool {
  return (Set.find(Item) != Set.end());
}

auto SetContainsParameter(const std::set<UnmaskedTestParameter>& ParamSet,
                          UnmaskedTestParameter Item) -> bool {
  return (ParamSet.find(Item) != ParamSet.end());
}