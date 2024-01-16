#pragma once

#include <string>

struct NameTestPair {
public:
    std::string Name;
    std::string Test;

    NameTestPair(std::string const& InName, std::string const& InTest) : Name(InName), Test(InTest) {}    
};