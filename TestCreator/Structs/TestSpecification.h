#pragma once

#include <string>

// A POD struct whch describes a test which has not been run yet. 
// This is expected to be used by a function which writes tests into
// a file.
// E.g. bool Test_ThisFunction() { ... }
// NOT UnmaskedAlwaysReturnValueTest(...);
struct TestSpecification {
public:
    std::string Filepath;
	std::string Type;

    TestSpecification(std::string const& InFilepath, std::string const& InType) : 
		Filepath(InFilepath),
		Type(InType) {} 
};