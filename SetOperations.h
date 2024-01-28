#pragma once

#include <set>
#include "UserlandIncludes/UnmaskedTests.h"

template <typename T, typename... Ts>
bool SetContainsItem(std::set<T, Ts...>& Set, T Item);

template <typename T, typename... Ts>
bool SetContainsItem(const std::set<T, Ts...>& ConstSet, T Item);

bool SetContainsParameter(const std::set<UnmaskedTestParameter>& ParamSet, UnmaskedTestParameter Item);