#pragma once

#include <string>

struct FixedArgument {
public:
    std::string Index;
    std::string Value;

    FixedArgument(std::string const& InIndex, std::string const& InValue) : Index(InIndex), Value(InValue) {}

    bool MatchesIndex(size_t InIndex) const {
        return (Index == std::to_string(InIndex));
    }
};

