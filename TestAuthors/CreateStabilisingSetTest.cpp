#include "CreateStabilisingSetTest.h"

#include <ctgmath>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>

#include "../StringOperations.h"
#include "NameTestPair.h"
#include "Logging.h"
#include <iostream>

std::string CreateStabilisingSetTest(std::vector<std::string> Tokens, std::string SourceFileAddress, std::size_t NumTestsToRun) {
    //# NEED TO PARSE THIS
    //# UnmaskedTestFixedPoints(std::function<int(int, int)>(&AddInts), 0, 0);

    using std::string;
    using std::to_string;
    using std::vector;
    using std::size_t;

    Log(std::cout, LOG, "Starting CreateStabilisingSetTest");

    // Before we start, we can make some asserts
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

    // TODO: Make ArgumentTypes vector<ArgumentType>
    // where ArgumentType contains the string and some functions
    // to do common manipulations
    // e.g. get the associated generator type and generator call
    vector<string> ArgumentTypes;


    // This is done with the weird if-statement to handle edge-cases easily.
    while (*CurrentToken != ")") {
        if (*CurrentToken != "," &&
            *CurrentToken != "(" &&
            *CurrentToken != ")")
            {
                ArgumentTypes.push_back(*CurrentToken);
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
    vector<string> FixedPointIndexes;
    vector<string> FixedPointArgumentValues;

    Log(std::cout, LOG, "Pulling Fixed Points");
    while (*CurrentToken != ")") { 
        if (*CurrentToken == ",") { CurrentToken++; }

        
        FixedPointIndexes.push_back(*CurrentToken);
        Log(std::cout, LOG, "Pulled fixed point index");
        Log(std::cout, VALUE_OUTPUT, *CurrentToken);
        // Skip the comma between arguments to the "template" function
        CurrentToken += 2;
        FixedPointArgumentValues.push_back(*CurrentToken);
        Log(std::cout, LOG, "Pulling fixed point argument value");
        Log(std::cout, VALUE_OUTPUT, *CurrentToken);
        CurrentToken++;
    }

    Log(std::cout, LOG, "Extracted data from specification.");

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
    TestSource = ReplaceAllInString(TestSource, "TEST_FN_NAME", "");

    // Fix the return type and argument types
    TestSource = ReplaceAllInString(TestSource, "RETURN_TYPE", ReturnType);
    TestSource = ReplaceAllInString(TestSource, "NUM_ARGUMENT_TYPES", to_string(ArgumentTypes.size()));

    //string ArgumentTypesAsString = JoinVectorOfStrings(ArgumentTypes, ",");
    string ArgumentTypesAsString = JoinVectorOfStrings(ArgumentTypes, ",");
    TestSource = ReplaceAllInString(TestSource, "ARGUMENT_TYPES", ArgumentTypesAsString);

    // Insert the final arguments to the functions signature
    TestSource = ReplaceAllInString(TestSource, "NUM_TESTS_TO_RUN", to_string(NumTestsToRun));

    // Create generators and generate values
    // We could use std::format from C++20 but Jamie wants this to compile under C++14-ish

    vector<string> GeneratorTypes;
    for (auto const& ArgType : ArgumentTypes) {
        GeneratorTypes.push_back("Generator_" + ArgType);
    }

    TestSource = ReplaceAllInString(TestSource, "GENERATOR_TYPES", JoinVectorOfStrings(GeneratorTypes, ","));

    /* CURRENT REWRITE 
    // Knowing the longest type lets us allocate a buffer for the format string safely.
    auto LongestType = std::max_element(ArgumentTypes.begin(), ArgumentTypes.end(),
                               [](const auto& a, const auto& b) {
                                   return a.size() < b.size();
                               });

    vector<string> GeneratorTypesSource;
    for (auto ArgType : ArgumentTypes) {
        // IF THIS IS CHANGED, MAKE SURE THE COMMENT ON THE GENERATION OF 
        // THE CODE TO PULL VALUES FROM THE GENERATOR TUPLE STILL MAKES SENSE.
        size_t LengthOfTemporaryString = sizeof("Generator_, ") + (*LongestType).size() + 1;
        vector<char> TemporaryString(LengthOfTemporaryString, '\0');
        //auto TemporaryString = (char*) malloc(LengthOfTemporaryString);

        // Slightly cursed to use the vector as a char[] with auto-malloc and auto-free
        // But it is allowed.
        snprintf(TemporaryString.data(), LengthOfTemporaryString, "Generator_%s, ", ArgType.c_str());

        // Here we turn the vector<char> -> char[] -> std::string
        GeneratorTypesSource.emplace_back(TemporaryString.data());
    }

    TestSource = ReplaceAllInString(TestSource, "GENERATOR_TYPES", JoinVectorOfStrings(GeneratorTypesSource, ""));
    */

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


    /* CURRENT REWRITE PART 1 
    vector<string> GeneratorOutputValuesSource;

    for (int i = 0; i < ArgumentTypes.size(); i++) {
        // The Log_10(ArgumentTypes.size()) is calculating the size of the string being added.
        // It's the maximal length of `to_string(i)` 
        // The extra plus 1s are for the null terminator and a safety mechanism against floating point weirdness.
        size_t LengthOfTemporaryString = sizeof("std::get<>(Generators).GenerateValue(), ") + ceil(log10(ArgumentTypes.size()) + 1) + 1;

        // See above generation of the Generator tuple.
        vector<char> TemporaryString(LengthOfTemporaryString, '\0');
        snprintf(TemporaryString.data(), LengthOfTemporaryString, "std::get<%d>(Generators).GenerateValue(), ", i);
        GeneratorOutputValuesSource.emplace_back(TemporaryString.data());
    }

    // GOVSS -> GeneratorOutputValuesSourceString
    // THESE COMMENTS ARE REFERENCED BY CGSS.

    // DO NOT USE GOVSS ELSEWHERE WITHOUT RENAMING IT
    // ITS FINE HERE
    const string GOVSS(JoinVectorOfStrings(GeneratorOutputValuesSource, ""));
    TestSource = ReplaceAllInString(TestSource, "GENERATORS_GET_VALUES", GOVSS);

    */

    // This would be a normal JoinVectorOfStrings and replace but the input data needs some amount of transformation
    // We know argument types (int, float, etc)
    // and we want Generator Types (Generator_int, Generator_float, etc)

    // Step 1: Transform argument types to appropriate generator constructor calls
    // Step 2: Join those generator constructor strings for substitution

    // NOTE: Is this vector just GeneratorTypes + "()" elementwise?
    vector<string> GeneratorsSource;
    GeneratorsSource.reserve(ArgumentTypes.size());    

    for (auto const& ArgType : ArgumentTypes) {
        GeneratorsSource.push_back("Generator_" + ArgType + "()");
    }

    TestSource = ReplaceAllInString(TestSource, "GENERATORS", JoinVectorOfStrings(GeneratorsSource, ","));

    vector<string> GetGeneratedArgumentsSource;
    GetGeneratedArgumentsSource.reserve(ArgumentTypes.size());

    // We name this counter LocalIndex because the `i` in the generated code refers to a variable
    // in the generated program.
    // (This code gets inserted into a for-loop)

    for (int LocalIndex = 0; LocalIndex < ArgumentTypes.size(); LocalIndex++) {
        auto FixedIndexIterator = std::find(FixedPointIndexes.begin(), FixedPointIndexes.end(), to_string(LocalIndex));
        // If this index is not a fixed point.
        if (FixedIndexIterator == FixedPointIndexes.end()) { 
            GetGeneratedArgumentsSource.push_back("std::get<" + to_string(LocalIndex) + ">(GeneratedArguments[i])");
        }
        // This is a fixed point
        else {
            // Convert iterator to size_t "C-style" index
            size_t FixedPointArgumentValueIndex = std::distance(FixedPointIndexes.begin(), FixedIndexIterator);
            GetGeneratedArgumentsSource.push_back(FixedPointArgumentValues[FixedPointArgumentValueIndex]);
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
        ArgumentOutputValues.push_back("\t\t\tstd::cout << std::get<" + to_string(LocalIndex) + ">(GeneratedArguments[i]); \n \t\t\tstd::cout << \",\";");
    }

    // TODO: MAKE THIS AWARE THAT SOME VALUES ARE FIXED
    // THE OUTPUT ISN'T USER FRIENDLY

    TestSource = ReplaceAllInString(TestSource, "COUT_EACH_ARGUMENT_IN_TURN", JoinVectorOfStrings(ArgumentOutputValues, "\n"));

    
    /* CURRENT REWRITE PART 2
    vector<string> CallGeneratorSource;
    for (auto ArgType : ArgumentTypes) {
        size_t LengthOfTemporaryString = sizeof("Generator_(), ") + (*LongestType).size() + 1;
        vector<char> TemporaryString(LengthOfTemporaryString, '\0');

        // Slightly cursed to use the vector as a char[] with auto-malloc and auto-free
        // But it is allowed.
        snprintf(TemporaryString.data(), LengthOfTemporaryString, "Generator_%s(), ", ArgType.c_str());

        

        // Here we turn the vector<char> -> char[] -> std::string
        CallGeneratorSource.emplace_back(TemporaryString.data());
    }

    // See GOVSS comment.
    // CGSS -> CallGeneratorSourceString
    string CGSS(JoinVectorOfStrings(CallGeneratorSource, ""));
    TestSource = ReplaceAllInString(TestSource, "GENERATORS", CGSS);    
    */
    
    //Log(std::cout, LOG, "-------> Test source from CreateStabilisingSetTest.cpp here");
    //Log(std::cout, VALUE_OUTPUT, TestSource);

    return TestSource;
    /*
    
    with open(f"./UnmaskedTests/{FunctionName}_TEST.cpp", mode="w+") as TestSourceFile:
        with open("Templates/StabilisingSetTestTemplate.cpp", mode="r") as Template:
            TestSource = Template.read()

            # Correct the import statement. We're including a .cpp file here, but we want the implementation for a
            # small, self-contained program so this is evil but acceptable.
            TestSource = TestSource.replace("FILE_ADDRESS", SourceFileAddress)

            # Set the function name
            #TestSource = TestSource.replace("TEST_FN_NAME", "")

            # Fix the return type and argument types
            TestSource = TestSource.replace("RETURN_TYPE", ReturnType)
            TestSource = TestSource.replace("NUM_ARGUMENT_TYPES", str(len(ArgumentTypes)))
            TestSource = TestSource.replace("ARGUMENT_TYPES", ",".join(ArgumentTypes))

            # Generate the rest of this functions signature
            OtherArguments = ""
            for i in range(len(FixedPointIndexes)):
                OtherArguments += f", size_t Index{i}, {ArgumentTypes[int(FixedPointIndexes[i])]} Value{i}"

            # Insert the final arguments to the functions signature
            TestSource = TestSource.replace("OTHER_ARGS", OtherArguments)
            TestSource = TestSource.replace("NUM_TESTS_TO_RUN", str(NumTestsToRun))

            # Create generators and generate values
            # Doing some trickery here with a list to make the syntax nicer
            GeneratorTypesSource = [f"Generator_{ArgType}, " for ArgType in ArgumentTypes]
            GeneratorTypesSource = "".join(GeneratorTypesSource)
            GeneratorTypesSource = GeneratorTypesSource[:-2]

            TestSource = TestSource.replace("GENERATOR_TYPES", GeneratorTypesSource)

            GeneratorsOutputValuesSource = [f"std::get<{i}>(Generators).GenerateValue(), " for i in range(len(ArgumentTypes))]
            GeneratorsOutputValuesSource = "".join(GeneratorsOutputValuesSource)
            GeneratorsOutputValuesSource = GeneratorsOutputValuesSource[:-2]
            TestSource = TestSource.replace("GENERATORS_GET_VALUES", GeneratorsOutputValuesSource)

            GeneratorsSource = [f"Generator_{ArgType}(), " for ArgType in ArgumentTypes]
            GeneratorsSource = "".join(GeneratorsSource)
            GeneratorsSource = GeneratorsSource[:-2]

            TestSource = TestSource.replace("GENERATORS", GeneratorsSource)            

            ValueGenerationSource = [f"Generator<{ArgType}>::GenerateValue(), " for ArgType in ArgumentTypes]
            ValueGenerationSource = "".join(ValueGenerationSource)
            # Remove the comma and space generated by the last argument type
            ValueGenerationSource = ValueGenerationSource[:-2]
            TestSource = TestSource.replace("GENERATE_ARGUMENTS", ValueGenerationSource)

            # Fill the arguments into the function call
            # Again using a list for nicer syntax
            ArgumentString = []
            for Index, Value in enumerate(ArgumentTypes):
                if Index not in FixedPointIndexes:
                    ArgumentString.append(f"std::get<{Index}>(GeneratedArguments[i]), ")
                else:
                    # Find appropriate value to fill in
                    ArgumentValueIndex = FixedPointIndexes.index(Index)
                    ArgumentString.append(f"{FixedPointArgumentValues[ArgumentValueIndex]}, ")

            # Join to string and remove final comma and space.
            ArgumentString = "".join(ArgumentString)
            ArgumentString = ArgumentString[:-2]

            TestSource = TestSource.replace("GENERATED_ARGUMENTS", ArgumentString)

            ArgumentOutputValuesInTurn = [f'\t\t\tstd::cout << std::get<{j}>(GeneratedArguments[i]); \n \t\t\tstd::cout << ","; \n ' for j in range(len(ArgumentTypes))]
            ArgumentOutputValuesInTurn = "".join(ArgumentOutputValuesInTurn)
            TestSource = TestSource.replace("COUT_EACH_ARGUMENT_IN_TURN", ArgumentOutputValuesInTurn)

            # Write in the function name
            TestSource = TestSource.replace("FN_NAME", FunctionName)

            # Actually write this data to the file
            TestSourceFile.write(TestSource)




    print("hi")*/
}

std::string CreateStabilisingSetTest(std::vector<std::string> Tokens, std::string SourceFileAddress) {
    return CreateStabilisingSetTest(Tokens, SourceFileAddress, DEFAULT_NUM_TESTS_TO_RUN);
}

std::vector<NameTestPair> CreateAllStabilisingTests(std::vector<std::string> Tokens, std::string SourceFileAddress, std::size_t NumTestsToRun) {
    auto CurrentToken = Tokens.begin();

    using std::vector;
    using std::string;

    vector<NameTestPair> ToReturn;
    // Potentially there should be a reservation step here.
    // Reserve space for 1 string per instance of the marker.

    CurrentToken = std::find(CurrentToken, Tokens.end(), STABILISING_TEST_MARKER);
    Log(std::cout, LOG, "If this CurrentToken is Tokens.end(), then no tests were found.");
    Log(std::cout, VALUE_OUTPUT, std::to_string(CurrentToken == Tokens.end()));

    int CurrentTestNumber = 0;

    while (CurrentToken != Tokens.end()) {
        auto EndingToken = std::find(CurrentToken, Tokens.end(), ";");

        vector<string> NeededTokens(CurrentToken, EndingToken);

        string TestName = SourceFileAddress + std::to_string(CurrentTestNumber);
        CurrentTestNumber++;
        string TestSource = CreateStabilisingSetTest(NeededTokens, SourceFileAddress, NumTestsToRun);

        ToReturn.emplace_back(TestName, TestSource);
        //ToReturn.push_back(CreateStabilisingSetTest(NeededTokens, SourceFileAddress, NumTestsToRun));

        CurrentToken = std::find(CurrentToken+1, Tokens.end(), STABILISING_TEST_MARKER);
    }

    return ToReturn;
}


std::vector<NameTestPair> CreateAllStabilisingTests(std::vector<std::string> Tokens, std::string SourceFileAddress) {
    return CreateAllStabilisingTests(Tokens, SourceFileAddress, DEFAULT_NUM_TESTS_TO_RUN);
}