#include "MapOperations.h"

#include <map>

// Amazingly the standard version of this is a C++20 feature.
// This is absolutely not worth requiring everyone's code to run
// under C++20.
template <typename K, typename... V>
auto MapContainsKey(std::map<K, V...>& Map, K Key) -> bool {
  return (Map.find(Key) != Map.end());
}