#include "SetOperations.h"

#include <set>
#include "UnmaskedTests.h"

template <typename T, typename... Ts>
bool SetContainsItem(std::set<T, Ts...>& Set, T Item) {
    return (Set.find(Item) != Set.end());
}

template <typename T, typename... Ts>
bool SetContainsItem(const std::set<T, Ts...>& Set, T Item) {
    return (Set.find(Item) != Set.end());
}

bool SetContainsParameter(const std::set<UnmaskedTestParameter>& ParamSet, UnmaskedTestParameter Item) {
    return (ParamSet.find(Item) != ParamSet.end());
}