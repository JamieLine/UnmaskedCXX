#ifndef TESTCREATOR_STRUCTS_FIXEDARGUMENT_H
#define TESTCREATOR_STRUCTS_FIXEDARGUMENT_H

#include <string>
#include <utility>

#include "ArgumentIndex.h"
#include "ArgumentValue.h"

// This is a POD struct used in the generation of tests which
// require an argument to be fixed across all runs in a batch.
struct FixedArgument {
 public:
  ArgumentIndex Index;
  ArgumentValue Value;

  FixedArgument(ArgumentIndex InIndex, ArgumentValue InValue)
      : Index(std::move(InIndex)), Value(std::move(InValue)) {}

  auto MatchesIndex(size_t InIndex) const -> bool {
    return (Index.Index == std::to_string(InIndex));
  }
};

#endif /* TESTCREATOR_STRUCTS_FIXEDARGUMENT_H */
