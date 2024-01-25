#pragma once

#include <string>

struct ArgumentType {
public:
    std::string Type;

    ArgumentType(std::string InType) : Type(InType) {}

    std::string GetGeneratorType() const { return "Generator_" + Type; }
    std::string GetGeneratorCall() const { return GetGeneratorType() + "()"; }
};