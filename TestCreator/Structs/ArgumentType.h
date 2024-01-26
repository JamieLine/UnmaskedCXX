#pragma once

#include <string>

// POD struct which holds the type of an argument and a few small utility methods
struct ArgumentType {
public:
    std::string Type;

    ArgumentType(std::string InType) : Type(InType) {}

    std::string GetGeneratorType() const { return "Generator_" + Type; }
    std::string GetGeneratorCall() const { return GetGeneratorType() + "()"; }
};