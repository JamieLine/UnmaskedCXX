#include "CreateAlwaysReturnValueTest.h"

#include <algorithm>
#include <ctgmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../Consts/TestMarkers.h"
#include "../Logging.h"
#include "../StringOperations.h"
#include "../Structs/ArgumentType.h"
#include "../Structs/FixedArgument.h"
#include "../Structs/TestSpecification.h"

auto CreateAlwaysReturnValueTest(
    std::vector<std::string>::iterator& FirstToken,
    GeneratorParameterStoreSeed Params, Filepath& TestDefinitionPath,
    const std::vector<std::string>& AdditionalIncludes,
    const std::string& GeneratedFunctionName, std::size_t NumTestsToRun)
    -> std::string {
  // Creates a test from Tokens extracted from input similar to the following:
  // `UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0,
  // 0, 0);`

  // Expects the following of the input.

  // 1. Begins with UnmaskedAlwaysReturnsValueTest (given in this file as
  // "ALWAYS_RETURN_VALUE_TEST_MARKER")
  // 2. The call to UnmaskedAlwaysReturnsValueTest takes in a
  // std::function<RETURN_TYPE(ARGUMENT_TYPES)> constructed by taking the
  // address of the tested function.
  // 3. The first non-function parameter is the expected return value.
  // 4. The expected return value is followed by Index-Value pairs.
  // In this example, the argument at index 0 is set to a value of 0.
  // If we had a 1 as the final argument, we would be setting the argument at
  // index 0 to have a value of 1.
  // 5. The last token should be ";".
  // 6. This string originates from a file with address `SourceFileAddress`,
  // where the implementation of the tested function can be found.

  using std::size_t;
  using std::string;
  using std::to_string;
  using std::vector;

  auto& CurrentToken = FirstToken;

  Log(std::cout, LOG, "Starting CreateAlwaysReturnValueTest");

  // Before we start, we should enforce that our assumptions on the input hold.
  Log(std::cout, LOG, "Validating that the first token is correct");
  if (*FirstToken != ALWAYS_RETURN_VALUE_TEST_MARKER) {
    Log(std::cout, ERROR,
        "Incorrect tokens passed into `CreateAlwaysReturnValueTest`");
    Log(std::cout, ERROR, "Outputting first value.");
    Log(std::cout, VALUE_OUTPUT, *FirstToken);
    // TODO(linej): Maybe this should signal to calling code that it failed?
    // Calling code can then take the generated output and treat it as
    // "suspicious"? Will need the return type to change from std::string to
    // something like a Pair<std::string, bool> where The bool is
    // "trustworthiness".
  } else {
    Log(std::cout, LOG, "First token is correct");
  }

  Log(std::cout, LOG, "Created CurrentToken iterator");

  // The first token was only used to specify which test to use.
  // The second token is an opening bracket
  // The third token is std::function
  // The fourth token is an opening angular bracket

  // These can all be discarded
  CurrentToken += 4;

  // After the opening angular bracket, we find the return type
  // The angular bracket contains something like <float(int, int)>
  // for a function `float F(int, int);`
  string ReturnType = *CurrentToken;
  CurrentToken++;

  Log(std::cout, LOG, "Pulled return type");
  Log(std::cout, VALUE_OUTPUT, ReturnType);

  vector<ArgumentType> ArgumentTypes;

  // On the first run through this loop, it should be true that
  // (*CurrentToken) == "("

  // This is done with the weird if-statement to handle edge-cases easily.
  // The characters that are checked for do not contain information, they are
  // simply delimiters between strings that do.
  while (*CurrentToken != ")") {
    if (*CurrentToken != "," && *CurrentToken != "(" && *CurrentToken != ")") {
      ArgumentTypes.emplace_back(*CurrentToken);
      Log(std::cout, LOG, "Pulled argument type");
      Log(std::cout, VALUE_OUTPUT, *CurrentToken);
    }
    CurrentToken++;
  }

  Log(std::cout, LOG, "Finished pulling argument types");

  // Now `CurrentToken` points to a closing bracket.
  // The next token is a closing angular bracket so we may skip that as well as
  // the opening bracket and ampersand immediately following.

  // UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0,
  // 0, 0);`
  //                                                          ^
  //                                                          ^
  // We are here.

  CurrentToken += 4;

  // UnmaskedAlwaysReturnsValueTest(std::function<int(int, int)>(&AddInts), 0,
  // 0, 0);`
  //                                                              ^^^^^^^
  //                                                              ^^^^^^^
  // Now we are here.

  Log(std::cout, LOG, "Pulling function name");
  string FunctionName = *CurrentToken;
  Log(std::cout, VALUE_OUTPUT, FunctionName);

  // Next two tokens are a closing bracket and a comma
  CurrentToken += 3;

  // Now we pick up the fixed points.

  // Read in the target value.
  string TargetValue = *CurrentToken;
  CurrentToken += 1;

  // The indexes should always neatly cast to `size_t` but this will be written
  // as a string into a FixedArgument instance. The conversion isn't valuable.
  vector<FixedArgument> FixedArguments;

  // Now there is either a comma, in which case there are more than zero fixed
  // arguments or a closing bracket, in which case there are zero

  if (*CurrentToken != ")") {
    // It's unknown (but knowable) how many fixed points we will define.
    // We just know that the last one will be followed by a ")".
    Log(std::cout, LOG, "Pulling Fixed Points");
    while (*CurrentToken != ")") {
      if (*CurrentToken == ",") {
        CurrentToken++;
      }

      string ThisPointIndex = *CurrentToken;

      Log(std::cout, LOG, "Pulled fixed point index");
      Log(std::cout, VALUE_OUTPUT, *CurrentToken);
      // Skip the comma between arguments to the "template" function
      CurrentToken += 2;

      string ThisPointValue = *CurrentToken;

      FixedArguments.emplace_back(ArgumentIndex(ThisPointIndex),
                                  ArgumentValue(ThisPointValue));

      Log(std::cout, LOG, "Pulling fixed point argument value");
      Log(std::cout, VALUE_OUTPUT, *CurrentToken);
      CurrentToken++;
    }
  }

  Log(std::cout, LOG, "Extracted data from specification.");

  // TODO(linej): We can do this entire function in one string allocation.
  // Read in a section of file Up To And Including a marker to substitute
  // Then Skip Forward the length of the buffer needed to hold the result
  // of the substitution.

  // For now, we simply read in the template, convert that stringstream to a
  // std::string and then perform a series of text substituions upon that
  // string.
  std::ifstream TemplateStream(
      "Templates/AlwaysReturnValueTestTemplate.cpp_template");
  std::stringstream Buffer;
  Buffer << TemplateStream.rdbuf();

  string TestSource = Buffer.str();

  TemplateStream.close();

  Log(std::cout, LOG, "Finished reading template.");

  // Correct the import statement. We're including a .cpp file here, but we want
  // the implementation for a small, self-contained program so this is evil but
  // acceptable.

  Log(std::cout, LOG, "Beginning substitutions.");

  // If `SourceFileAddress` begins with ./, we need to swap that with ../
  // As the tests are sitting one level higher in the directory tree.

  // As the template has the ../, this is done by removing it from
  // `SourceFileAddress`.
  if (TestDefinitionPath.Path.find("./", 0) == 0) {
    TestDefinitionPath.Path.erase(0, 2);
  }
  TestSource = ReplaceAllInString(TestSource, "FILE_ADDRESS",
                                  string(TestDefinitionPath));

  // Set the function name
  TestSource =
      ReplaceAllInString(TestSource, "TEST_FN_NAME", GeneratedFunctionName);

  // Add additional includes
  TestSource =
      ReplaceAllInString(TestSource, "ADDITIONAL_INCLUDES",
                         JoinVectorOfStrings(AdditionalIncludes, "\n"));

  // Fix the return type and argument types
  TestSource = ReplaceAllInString(TestSource, "RETURN_TYPE", ReturnType);
  TestSource = ReplaceAllInString(TestSource, "NUM_ARGUMENT_TYPES",
                                  to_string(ArgumentTypes.size()));

  // TODO(linej): Maybe make this not involve copying the entire ArgumentTypes
  // vector? Can use std::transform Or make JoinVectorOfStrings accept a
  // vector<ArgumentType> parameter as ArgumentType is just `std::string` with
  // extra utilities.
  vector<string> ArgumentTypeStrings;
  ArgumentTypeStrings.reserve(ArgumentTypes.size());

  std::transform(
      ArgumentTypes.begin(), ArgumentTypes.end(),
      std::back_inserter(ArgumentTypeStrings),
      [](const ArgumentType& ArgType) -> string { return string(ArgType); });

  // for (auto const& ArgType : ArgumentTypes) {
  //   ArgumentTypeStrings.emplace_back(ArgType);
  // }

  string ArgumentTypesAsString = JoinVectorOfStrings(ArgumentTypeStrings, ",");
  TestSource =
      ReplaceAllInString(TestSource, "ARGUMENT_TYPES", ArgumentTypesAsString);

  // Insert the final arguments to the functions signature
  TestSource = ReplaceAllInString(TestSource, "NUM_TESTS_TO_RUN",
                                  to_string(NumTestsToRun));

  // Create generators and generate values
  // We could use std::format from C++20 but Jamie wants this to compile under
  // C++14-ish

  // Again, this could be made easier by having JoinVectorOfStrings directly
  // read this data out of the array.
  vector<string> GeneratorTypes;
  GeneratorTypes.reserve(ArgumentTypes.size());

  std::transform(ArgumentTypes.begin(), ArgumentTypes.end(),
                 std::back_inserter(GeneratorTypes),
                 [](const ArgumentType& ArgType) -> string {
                   return ArgType.GetGeneratorType();
                 });

  // for (auto const& ArgType : ArgumentTypes) {
  //   GeneratorTypes.push_back(ArgType.GetGeneratorType());
  // }

  TestSource = ReplaceAllInString(TestSource, "GENERATOR_TYPES",
                                  JoinVectorOfStrings(GeneratorTypes, ","));

  TestSource =
      ReplaceAllInString(TestSource, "PUSH_PARAMETERS_TO_STORE",
                         Params.CreateGeneratorParameterStoreDefinition());

  // This would be a normal JoinVectorOfStrings and replace but the input data
  // needs some amount of transformation

  // We have a list of Argument Types
  // We wish to map each index into that list to a string
  // Which Python3 would call
  // f"std::get<{Index}>(Generators).GenerateValue(Parameters, {IsUsed})"
  // which is the actual code we wish to subsitute into the template.

  // Again, std::format isn't used as it's a C++20 feature.
  // And in this case, the format string is just adding strings into the middle.
  // So they aren't too bad to write as "string sums".

  vector<string> GetValueFromGeneratorCalls;
  GetValueFromGeneratorCalls.reserve(ArgumentTypes.size());

  for (int i = 0; i < ArgumentTypes.size(); i++) {
    // Maybe the ArgumentType should know its own index?
    // But that might require specialising `vector<ArgumentType>` or replacing
    // it.

    bool IsUsed = (std::find_if(FixedArguments.begin(), FixedArguments.end(),
                                [i](const FixedArgument& FArg) -> bool {
                                  return FArg.MatchesIndex(i);
                                }) == FixedArguments.end());

    GetValueFromGeneratorCalls.push_back(
        "std::get<" + to_string(i) +
        ">(Generators).GenerateValue(Parameters, " +
        std::to_string(static_cast<int>(IsUsed)) + ")");
  }

  TestSource =
      ReplaceAllInString(TestSource, "GENERATORS_GET_VALUES",
                         JoinVectorOfStrings(GetValueFromGeneratorCalls, ","));

  // This would be a normal JoinVectorOfStrings and replace but the input data
  // needs some amount of transformation We know argument types (int, float,
  // etc) and we want Generator Types (Generator_int, Generator_float, etc)

  // Step 1: Transform argument types to appropriate generator constructor calls
  // Step 2: Join those generator constructor strings for substitution

  vector<string> GeneratorsSource;
  GeneratorsSource.reserve(ArgumentTypes.size());

  std::transform(ArgumentTypes.begin(), ArgumentTypes.end(),
                 std::back_inserter(GeneratorsSource),
                 [](const ArgumentType& ArgType) -> string {
                   return ArgType.GetGeneratorCall();
                 });

  // for (auto const& ArgType : ArgumentTypes) {
  //   GeneratorsSource.push_back(ArgType.GetGeneratorCall());
  // }

  TestSource = ReplaceAllInString(TestSource, "GENERATORS",
                                  JoinVectorOfStrings(GeneratorsSource, ","));

  vector<string> GetGeneratedArgumentsSource;
  GetGeneratedArgumentsSource.reserve(ArgumentTypes.size());

  // We name this counter LocalIndex because the `i` in the generated code
  // refers to a variable in the generated program. (This code gets inserted
  // into a for-loop)

  for (int LocalIndex = 0; LocalIndex < ArgumentTypes.size(); LocalIndex++) {
    // Determine if we're investigating a fixed argument, and if we are, what
    // that argument is.
    auto FixedArgIterator =
        std::find_if(FixedArguments.begin(), FixedArguments.end(),
                     [LocalIndex](FixedArgument const& FArgs) -> bool {
                       return FArgs.MatchesIndex(LocalIndex);
                     });
    // If this is not a fixed arg
    if (FixedArgIterator == FixedArguments.end()) {
      GetGeneratedArgumentsSource.push_back(
          "std::get<" + to_string(LocalIndex) + ">(GeneratedArguments[i])");
    }
    // If this is a fixed arg
    else {
      GetGeneratedArgumentsSource.emplace_back((*FixedArgIterator).Value);
    }
  }

  TestSource =
      ReplaceAllInString(TestSource, "GENERATED_ARGUMENTS",
                         JoinVectorOfStrings(GetGeneratedArgumentsSource, ","));

  TestSource = ReplaceAllInString(TestSource, "FN_NAME", FunctionName);

  // This will contain a series of lines which take the arguments passed into
  // the tested function and outputs them to the console.
  vector<string> TestedFunctionArguments;
  TestedFunctionArguments.reserve(ArgumentTypes.size());

  // Here `i` refers to a for-loop in the generated code
  // LocalIndex is the index in the for-loop local to this file and this program
  // The \",\";" is because we need to construct the source for a string literal
  // at the end of a local string literal. At some point it might be worth
  // making a COMMA variable just to remove that hideousness.

  for (int LocalIndex = 0; LocalIndex < ArgumentTypes.size(); LocalIndex++) {
    // Determine if we're investigating a fixed argument, and if we are, what
    // that argument is.
    // TODO(linej): If the number of fixed arguments becomes large enough, it'll
    // become worth the investment to begin the next search just after the
    // position of the last fixed argument, and enforce that FixedArguments is
    // sorted with respect to index.
    auto FixedArgIterator =
        std::find_if(FixedArguments.begin(), FixedArguments.end(),
                     [LocalIndex](FixedArgument const& FArgs) -> bool {
                       return FArgs.MatchesIndex(LocalIndex);
                     });

    // In the following strings, the three "\t" chars are there to ensure that
    // the generated code has the indentation expected of human-written code. If
    // this is not a fixed argument.
    if (FixedArgIterator == FixedArguments.end()) {
      TestedFunctionArguments.push_back(
          "\t\t\tstd::cout << std::get<" + to_string(LocalIndex) +
          ">(GeneratedArguments[i]); \n \t\t\tstd::cout << \",\";");
    } else {
      TestedFunctionArguments.push_back("\t\t\tstd::cout << " +
                                        string((*FixedArgIterator).Value) +
                                        ";\n \t\t\tstd::cout << \",\";");
    }
  }

  TestSource =
      ReplaceAllInString(TestSource, "COUT_EACH_ARGUMENT_IN_TURN",
                         JoinVectorOfStrings(TestedFunctionArguments, "\n"));

  // TargetValue here is the value we expect the tested function to always
  // return.
  TestSource = ReplaceAllInString(TestSource, "TARGET_VALUE", TargetValue);

  return TestSource;
}
