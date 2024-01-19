#include "CreateStabilisingSetTest.h"

#include <ctgmath>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>

#include "../StringOperations.h"
#include "TestSpecification.h"
#include "FixedArgument.h"
#include "Logging.h"
#include <iostream>
#include "ArgumentType.h"

std::string CreateStabilisingSetTest(std::vector<std::string> Tokens, std::string SourceFileAddress, std::string OutputFileNameNoExtension, std::size_t NumTestsToRun) {
    //# NEED TO PARSE THIS
    //# UnmaskedTestFixedPoints(std::function<int(int, int)>(&AddInts), 0, 0);

    using std::string;
    using std::to_string;
    using std::vector;
    using std::size_t;

    Log(std::cout, LOG, "Starting CreateStabilisingSetTest");

    // Before we start, we can make some assertions
    Log(std::cout, LOG, "Validating that beginning and end of `Tokens` is correct");
    if (Tokens[0] != STABILISING_TEST_MARKER || Tokens[Tokens.size()-1] != ";") {
        Log(std::cout, ERROR, "Incorrect tokens passed into `CreateStabilisingSetTest`");
        Log(std::cout, ERROR, "Outputting first and last value.");
        Log(std::cout, VALUE_OUTPUT, Tokens[0]);
        Log(std::cout, VALUE_OUTPUT, Tokens[Tokens.size()-1]);
    }
    else {
        Log(std::cout, LOG, "Beginning and end of `Tokens` is correct");
    }

    auto CurrentToken = Tokens.begin();

    Log(std::cout, LOG, "Created CurrentToken iterator");


    // The first token was only used to specify which test to use.
    // The second token is an opening bracket
    // The third token is std::function
    // The fourth token is an opening angular bracket

    // These can all be discarded
    CurrentToken += 4;

    string ReturnType = *CurrentToken;
    CurrentToken++;

    Log(std::cout, LOG, "Pulled return type");
    Log(std::cout, VALUE_OUTPUT, ReturnType);

    vector<ArgumentType> ArgumentTypes;


    // This is done with the weird if-statement to handle edge-cases easily.
    while (*CurrentToken != ")") {
        if (*CurrentToken != "," &&
            *CurrentToken != "(" &&
            *CurrentToken != ")")
            {
                ArgumentTypes.emplace_back(*CurrentToken);
                Log(std::cout, LOG, "Pulled argument type");
                Log(std::cout, VALUE_OUTPUT, *CurrentToken);
        }
        CurrentToken++;
    }

    Log(std::cout, LOG, "Finished pulling argument types");

    // Now `CurrentToken` points to a closing bracket. 
    // The next token is a closing angular bracket so we may skip that as well as the opening bracket and ampersand immediately following.

    CurrentToken += 4;
    
    //Next is the function name

    Log(std::cout, LOG, "Pulling function name");
    string FunctionName = *CurrentToken;
    Log(std::cout, VALUE_OUTPUT, FunctionName);
    
    // Next two tokens are a closing bracket and a comma
    CurrentToken += 3;

    // Now we pick up the fixed points.

    // The indexes should always neatly cast to `size_t` but this will be written as a string.
    // The conversion isn't valuable.
    // This should be merged into a single FixedArgument class holding the two strings together
    // These are 2 objects but shouldn't be.
    // FixedPointArgumentValues is useless without the index array.

    vector<FixedArgument> FixedArguments;

    Log(std::cout, LOG, "Pulling Fixed Points");
    while (*CurrentToken != ")") { 
        if (*CurrentToken == ",") { CurrentToken++; }

        string ThisPointIndex = *CurrentToken;

        Log(std::cout, LOG, "Pulled fixed point index");
        Log(std::cout, VALUE_OUTPUT, *CurrentToken);
        // Skip the comma between arguments to the "template" function
        CurrentToken += 2;

        string ThisPointValue = *CurrentToken;

        FixedArguments.emplace_back(ThisPointIndex, ThisPointValue);

        Log(std::cout, LOG, "Pulling fixed point argument value");
        Log(std::cout, VALUE_OUTPUT, *CurrentToken);
        CurrentToken++;
    }

    Log(std::cout, LOG, "Extracted data from specification.");


    // TODO: We can do this entire function in one string allocation.
    // Read in a section of file Up To And Including a marker to substitute
    // Then Skip Forward the length of the buffer needed to hold the result 
    // of the substitution.
    std::ifstream TemplateStream("Templates/StabilisingSetTestTemplate.cpp");
    std::stringstream Buffer;
    Buffer << TemplateStream.rdbuf();

    string TestSource = Buffer.str();

    TemplateStream.close();

    Log(std::cout, LOG, "Finished reading template.");

    // Correct the import statement. We're including a .cpp file here, but we want the implementation for a
    // small, self-contained program so this is evil but acceptable.

    // TODO: Should this include a H file and add the cpp to CMAKE?

    Log(std::cout, LOG, "Beginning substitutions.");

    // If `SourceFileAddress` begins with ./, we need to swap that with ../
    // As the tests are sitting one level higher in the directory tree.

    // As the template has the ../, this is done by removing it.
    if (SourceFileAddress.find("./", 0) == 0) {
        SourceFileAddress.erase(0, 2);
    }
    TestSource = ReplaceAllInString(TestSource, "FILE_ADDRESS", SourceFileAddress);

    // Set the function name
    // TODO: WHY IS THIS EMPTY?
    TestSource = ReplaceAllInString(TestSource, "TEST_FN_NAME", OutputFileNameNoExtension);

    // Fix the return type and argument types
    TestSource = ReplaceAllInString(TestSource, "RETURN_TYPE", ReturnType);
    TestSource = ReplaceAllInString(TestSource, "NUM_ARGUMENT_TYPES", to_string(ArgumentTypes.size()));

    // TODO: Maybe make this not involve copying the entire ArgumentTypes vector? Can use std::transform
    vector<string> ArgumentTypeStrings;
    ArgumentTypeStrings.reserve(ArgumentTypes.size());
    
    for (auto const& ArgType : ArgumentTypes) {
        ArgumentTypeStrings.push_back(ArgType.Type);
    }

    string ArgumentTypesAsString = JoinVectorOfStrings(ArgumentTypeStrings, ",");
    TestSource = ReplaceAllInString(TestSource, "ARGUMENT_TYPES", ArgumentTypesAsString);

    // Insert the final arguments to the functions signature
    TestSource = ReplaceAllInString(TestSource, "NUM_TESTS_TO_RUN", to_string(NumTestsToRun));

    // Create generators and generate values
    // We could use std::format from C++20 but Jamie wants this to compile under C++14-ish

    vector<string> GeneratorTypes;
    for (auto const& ArgType : ArgumentTypes) {
        GeneratorTypes.push_back(ArgType.GetGeneratorType());
    }

    TestSource = ReplaceAllInString(TestSource, "GENERATOR_TYPES", JoinVectorOfStrings(GeneratorTypes, ","));

    // This would be a normal JoinVectorOfStrings and replace but the input data needs some amount of transformation
    
    // We have a list of Argument Types
    // We wish to map each index into that list to a string
    // Which Python3 would call
    // f"std::get<{Index}>(Generators).GenerateValue()"
    // which is the actual code we wish to subsitute into the template.

    vector<string> GetValueFromGeneratorCalls;
    GetValueFromGeneratorCalls.reserve(ArgumentTypes.size());

    for (int i = 0; i < ArgumentTypes.size(); i++) {
        GetValueFromGeneratorCalls.push_back("std::get<" + to_string(i) + ">(Generators).GenerateValue()");
    }

    TestSource = ReplaceAllInString(TestSource,
                    "GENERATORS_GET_VALUES",
                    JoinVectorOfStrings(GetValueFromGeneratorCalls, ","));

    // This would be a normal JoinVectorOfStrings and replace but the input data needs some amount of transformation
    // We know argument types (int, float, etc)
    // and we want Generator Types (Generator_int, Generator_float, etc)

    // Step 1: Transform argument types to appropriate generator constructor calls
    // Step 2: Join those generator constructor strings for substitution

    vector<string> GeneratorsSource;
    GeneratorsSource.reserve(ArgumentTypes.size());    

    for (auto const& ArgType : ArgumentTypes) {
        GeneratorsSource.push_back(ArgType.GetGeneratorCall());
    }

    TestSource = ReplaceAllInString(TestSource, "GENERATORS", JoinVectorOfStrings(GeneratorsSource, ","));

    vector<string> GetGeneratedArgumentsSource;
    GetGeneratedArgumentsSource.reserve(ArgumentTypes.size());

    // We name this counter LocalIndex because the `i` in the generated code refers to a variable
    // in the generated program.
    // (This code gets inserted into a for-loop)

    for (int LocalIndex = 0; LocalIndex < ArgumentTypes.size(); LocalIndex++) {
        // Determine if we're investigating a fixed argument, and if we are, what that argument is.
        auto FixedArgIterator = std::find_if( FixedArguments.begin(),
                                                FixedArguments.end(),
                                                [LocalIndex](FixedArgument const& F) {return F.MatchesIndex(LocalIndex); });
        // If this is not a fixed arg
        if (FixedArgIterator == FixedArguments.end()) {
            GetGeneratedArgumentsSource.push_back("std::get<" + to_string(LocalIndex) + ">(GeneratedArguments[i])");
        }
        // If this is a fixed arg
        else {
            GetGeneratedArgumentsSource.push_back((*FixedArgIterator).Value);
        }

    }

    TestSource = ReplaceAllInString(TestSource, "GENERATED_ARGUMENTS", JoinVectorOfStrings(GetGeneratedArgumentsSource, ","));

    TestSource = ReplaceAllInString(TestSource, "FN_NAME", FunctionName);

    
    vector<string> ArgumentOutputValues;
    ArgumentOutputValues.reserve(ArgumentTypes.size());

    // Here `i` refers to a for-loop in the generated code
    // LocalIndex is the index in the for-loop local to this file and this program
    // The \",\";" is because we need to construct the source for a string literal at the end of a local string literal.
    // At some point it might be worth making a COMMA variable just to remove that hideousness.

    for (int LocalIndex = 0; LocalIndex < ArgumentTypes.size(); LocalIndex++) {
        // Determine if we're investigating a fixed argument, and if we are, what that argument is.
        auto FixedArgIterator = std::find_if( FixedArguments.begin(),
                                                FixedArguments.end(),
                                                [LocalIndex](FixedArgument const& F) {return F.MatchesIndex(LocalIndex); });

        // If this is not a fixed argument.
        if (FixedArgIterator == FixedArguments.end()) {
            ArgumentOutputValues.push_back("\t\t\tstd::cout << std::get<" + to_string(LocalIndex) + ">(GeneratedArguments[i]); \n \t\t\tstd::cout << \",\";");
        }
        else {
            ArgumentOutputValues.push_back("\t\t\tstd::cout << " + (*FixedArgIterator).Value + ";\n \t\t\tstd::cout << \",\";");
        }
    }

    TestSource = ReplaceAllInString(TestSource, "COUT_EACH_ARGUMENT_IN_TURN", JoinVectorOfStrings(ArgumentOutputValues, "\n"));

    return TestSource;
}

std::string CreateStabilisingSetTest(std::vector<std::string> Tokens, std::string SourceFileAddress, std::string OutputFileNameNoExtension) {
    return CreateStabilisingSetTest(Tokens, SourceFileAddress, OutputFileNameNoExtension, DEFAULT_NUM_TESTS_TO_RUN);
}

std::vector<TestSpecification> CreateAllStabilisingTests(std::vector<std::string> Tokens, std::string SourceFileAddress, std::size_t NumTestsToRun) {
    auto CurrentToken = Tokens.begin();

    using std::vector;
    using std::string;

    vector<TestSpecification> ToReturn;
    // Potentially there should be a reservation step here.
    // Reserve space for 1 string per instance of the marker.

    CurrentToken = std::find(CurrentToken, Tokens.end(), STABILISING_TEST_MARKER);
    Log(std::cout, LOG, "If this CurrentToken is Tokens.end(), then no tests were found.");
    Log(std::cout, VALUE_OUTPUT, std::to_string(CurrentToken == Tokens.end()));

    int CurrentTestNumber = 0;

    while (CurrentToken != Tokens.end()) {
        auto EndingToken = std::find(CurrentToken, Tokens.end(), ";");

        vector<string> NeededTokens(CurrentToken, EndingToken);

        string TestName = SourceFileAddress;
        

        // Clean the source file address
        // Then use that to generate the name of the generated test function.
        const std::vector<std::string> PartsToRemove = {"../", "./", ".h", ".hpp", ".cxx", ".c", ".cpp", ".hxx"};
		for (auto const& Extension : PartsToRemove) {
			std::string::size_type Index = TestName.find(Extension);

			if (Index != std::string::npos)
   				TestName.erase(Index, Extension.length());
		}

        
        // Replace `/` in the directory tree with `_` to avoid Directory Trickery
		TestName = ReplaceAllInString(TestName, "/", "_");
		TestName = ReplaceAllInString(TestName, "\\", "_"); 
        
        TestName += std::to_string(CurrentTestNumber);
        CurrentTestNumber++;

        string TestSource = CreateStabilisingSetTest(NeededTokens, SourceFileAddress, TestName, NumTestsToRun);

        ToReturn.emplace_back(TestName, STABILISING_TEST_MARKER, TestSource);

        CurrentToken = std::find(CurrentToken+1, Tokens.end(), STABILISING_TEST_MARKER);
    }

    return ToReturn;
}


std::vector<TestSpecification> CreateAllStabilisingTests(std::vector<std::string> Tokens, std::string SourceFileAddress) {
    return CreateAllStabilisingTests(Tokens, SourceFileAddress, DEFAULT_NUM_TESTS_TO_RUN);
}