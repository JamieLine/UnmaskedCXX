#pragma once

#include <string>

// A POD struct whch describes a test which has not been run yet. 
// This is expected to be used by a function which writes tests into
// a file.
// The source code should be the Full Source Code for a test, not the 
// source code of the specification from the user.
// E.g. bool Test_ThisFunction() { ... }
// NOT UnmaskedAlwaysReturnValueTest(...);
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