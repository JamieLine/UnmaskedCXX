#pragma once

#include <map>

// Amazingly the standard version of this is a C++20 feature.
// This is absolutely not worth requiring everyone's code to run
// under C++20.
template <typename K, typename... V>
bool MapContainsKey(std::map<K, V...>& Map, K Key);