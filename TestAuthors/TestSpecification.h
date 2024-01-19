#pragma once

#include <string>

struct TestSpecification {
public:
    std::string Name;
	std::string Type;
	std::string SourceCode;

    TestSpecification(std::string const& InName, std::string const& InType, std::string const& InSource) : 
		Name(InName),
		Type(InType),
		SourceCode(InSource) {} 
};