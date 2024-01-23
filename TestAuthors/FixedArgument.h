#pragma once

#include <string>


// This is a POD struct used in the generation of tests which
// require an argument to be fixed across all runs in a batch.
struct FixedArgument {
public:
    std::string Index;
    std::string Value;

    FixedArgument(std::string const& InIndex, std::string const& InValue) : Index(InIndex), Value(InValue) {}

    bool MatchesIndex(size_t InIndex) const {
        return (Index == std::to_string(InIndex));
    }
};

