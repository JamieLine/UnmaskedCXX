#include "WriteUnmaskedPredicateTest.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "AdvancedLogging.h"
#include "MapOperations.h"
#include "StringOperations.h"
#include "TestCreator/Parsing/AdvancedLoggingWithBrackets.h"
#include "TestCreator/Structs/GeneratorSettingDescriptor.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TestCreationStatus.h"
#include "TestCreator/Writing/ReplaceSymbolAndLog.h"
#include "TestCreator/Writing/Templates.h"

auto WriteUnmaskedPredicateTest(const TestCreationContext& Context,
                                const ParsedUnmaskedPredicateTest& ToWrite)
    -> WithStatus<std::string> {
  WritingLogging.Log(std::cout, true,
                     "Beginning to write UnmaskedPredicateTest");
  WritingLogging.IncreaseIndentationLevel();

  auto SkeletonContent = TestSkeleton.ReadContentsOfFile();
  if (!SkeletonContent.DataExists) {
    WritingLogging.DecreaseIndentationLevel();
    WritingLogging.Log(std::cout, false, "Could not read TestSkeleton");
    WritingLogging.OutputValue(std::cout, TestSkeleton.Path);
    return {"", TestCreationStatus::COULD_NOT_READ_INPUT_FILE};
  }

  std::string ToReturn = SkeletonContent.Data;

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "FILE_ADDRESS",
                                 Context.TestDefinitionPath.Path);

  ToReturn = ReplaceSymbolAndLog(
      std::cout, ToReturn, "ADDITIONAL_INCLUDES",
      JoinVectorOfStrings(Context.AdditionalIncludes, "\n"));

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "TEST_FN_NAME",
                                 Context.GeneratedFunctionName);

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "NUM_TESTS_TO_RUN",
                                 std::to_string(Context.NumTestsToRun));

  ToReturn = ReplaceSymbolAndLog(
      std::cout, ToReturn, "ARGUMENT_TYPES",
      JoinVectorOfStrings(ToWrite.TestedFunction.ArgumentTypes, ", "));

  // Handling the generator types is slightly more
  // awkward, because the user may specify zero or
  // more of those types We'll work out a sensible
  // default, and if the user has passed in a
  // non-empty replacement, we'll use that instead
  std::vector<std::string> GeneratorTypes;

  std::transform(
      ToWrite.TestedFunction.ArgumentTypes.begin(),
      ToWrite.TestedFunction.ArgumentTypes.end(),
      std::back_inserter(GeneratorTypes),
      [](std::string const& S) -> std::string { return "Generator_" + S; });

  // TODO: This generator setting handler should be its own section.

  if (!ToWrite.GeneratorSettings.Settings.empty()) {
    auto& AllGeneratorSettings = ToWrite.GeneratorSettings.Settings;

    for (auto GeneratorSettingMap : AllGeneratorSettings) {
      // Every Generator Setting must have an
      // index
      if (!MapContainsKey(GeneratorSettingMap, std::string("Index"))) {
        WritingLogging.DecreaseIndentationLevel();
        WritingLogging.Log(
            std::cout, false,
            "Tried to read GeneratorSettings, but found one without an index.");
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      // A Generator Setting without a
      // GeneratorType is valid, its just
      // uninteresting here
      if (!MapContainsKey(GeneratorSettingMap, std::string("GeneratorType"))) {
        continue;
      }

      // This is just a long way of saying
      // "GeneratorTypes[Index] = GivenType"
      int Index;

      // TODO: THIS SHOULD PROBABLY BE IN A
      // SEPARATE VALIDATION STEP ALONG WITH
      // SEVERAL OTHER VALIDATIONS AND SANITY
      // CHECKS AS SHOULD THE STUFF ABOVE
      try {
        Index = std::stoi(GeneratorSettingMap["Index"]);
      } catch (std::invalid_argument const& Exception) {
        WritingLogging.Log(std::cout, false,
                           "WriteUnmaskedPredicateTest found a "
                           "non-integer index.");
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      } catch (std::out_of_range const& Exception) {
        WritingLogging.Log(std::cout, false,
                           "WriteUnmaskedPredicateTest found an "
                           "integer but it was out of "
                           "range to fit in an int.");
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      if (Index < 0 || Index >= GeneratorTypes.size()) {
        WritingLogging.Log(std::cout, false,
                           "WriteUnmaskedPredicateTest was "
                           "given an index which was a valid "
                           "integer but outside of the bounds "
                           "of the GeneratorTypes vector. "
                           "Index first, GeneratorTypes size "
                           "second.");
        WritingLogging.OutputValue(std::cout, std::to_string(Index));
        WritingLogging.OutputValue(std::cout,
                                   std::to_string(GeneratorTypes.size()));
        WritingLogging.OutputValue(
            std::cout,
            std::to_string(ToWrite.TestedFunction.ArgumentTypes.size()));
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      // Clean off any ""
      GeneratorTypes[Index] =
          ReplaceAllInString(GeneratorSettingMap["GeneratorType"], "\"", "");
    }
  }

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "GENERATOR_TYPES",
                                 JoinVectorOfStrings(GeneratorTypes, ", "));

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "RETURN_TYPE",
                                 ToWrite.TestedFunction.ReturnType);

  ToReturn = ReplaceSymbolAndLog(
      std::cout, ToReturn, "PUSH_PARAMETERS_TO_STORE",
      Context.Params.CreateGeneratorParameterStoreDefinition());

  // Here we write source code to get the values from the generators
  std::vector<std::string> GeneratorGetValues;

  const auto& ArgumentTypes = ToWrite.TestedFunction.ArgumentTypes;
  GeneratorGetValues.reserve(ArgumentTypes.size());

  for (int i = 0; i < ArgumentTypes.size(); i++) {
    const auto& GenSettings = ToWrite.GeneratorSettings.Settings;
    bool IsUsed =
        std::find_if(
            GenSettings.begin(), GenSettings.end(),
            [i](const GeneratorSettingDescriptor& Setting) -> bool {
              if (!MapContainsKey(Setting, std::string("Index"))) {
                ParsingLogging.Log(
                    std::cout, false,
                    "Found GeneratorSettingsDescriptor without an index");
                return false;
              }

              // TODO: THIS IS PROBABLY DUMB? SHOULD WE CONVERT THE SETTING TO
              // INT?
              if (Setting.at("Index") != std::to_string(i)) {
                return false;
              }

              if (!MapContainsKey(Setting, std::string("Fixed"))) {
                return false;
              }

              return Setting.at("Fixed") == "true";
            }) == GenSettings.end();

    GeneratorGetValues.push_back("std::get<" + std::to_string(i) +
                                 ">(Generators).GenerateValue(Parameters, " +
                                 std::to_string(IsUsed) + ")");
  }

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "GENERATORS_GET_VALUES",
                                 JoinVectorOfStrings(GeneratorGetValues, ","));

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "FN_NAME",
                                 ToWrite.TestedFunction.Name);

  std::vector<std::string> GeneratedArguments;

  for (int i = 0; i < ArgumentTypes.size(); i++) {
    GeneratedArguments.push_back("std::get<" + std::to_string(i) +
                                 ">(GeneratedArguments[i])");
  }
  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "GENERATED_ARGUMENTS",
                                 JoinVectorOfStrings(GeneratedArguments, ", "));

  std::vector<std::string> PassedArguments;

  for (auto _ : GeneratedArguments) {
    PassedArguments.push_back("");
  }

  std::vector<std::string> LoggingArgumentsParts;

  for (int i = 0; i < GeneratedArguments.size(); i++) {
    LoggingArgumentsParts.push_back("std::to_string(std::get<" +
                                    std::to_string(i) +
                                    ">(PassedArguments[i]))");
  }

  ToReturn = ReplaceSymbolAndLog(
      std::cout, ToReturn, "LOGGING_ARGUMENTS",
      JoinVectorOfStrings(LoggingArgumentsParts, " + \", \" + "));

  std::vector<std::string> GeneratorScriptLambdas;
  std::size_t CurrentLambdaCounter = 0;

  for (auto& Setting : ToWrite.GeneratorSettings.Settings) {
    // We've already checked that it has an index.

    if (MapContainsKey(Setting, std::string("GeneratorScript"))) {
      // TODO: Check that this doesn't cause a crash
      int Index = std::stoi(Setting.at("Index"));

      // TODO: The parser should have probably already removed the quotes we're
      // targetting.
      //

      std::string CleanedGeneratorScript = Setting.at("GeneratorScript");
      CleanedGeneratorScript =
          CleanedGeneratorScript.substr(1, CleanedGeneratorScript.size() - 2);

      GeneratorScriptLambdas.push_back("auto GeneratorScript_" +
                                       std::to_string(CurrentLambdaCounter) +
                                       " = " + CleanedGeneratorScript + ";");

      PassedArguments[Index] =
          "GeneratorScript_" + std::to_string(CurrentLambdaCounter) + "()";
      CurrentLambdaCounter++;
    }

    if (MapContainsKey(Setting, std::string("Value"))) {
      int Index = std::stoi(Setting.at("Index"));
      // We need to clean some quotes off of the input
      //
      std::string Value = Setting.at("Value");
      Value = Value.substr(1, Value.size() - 2);
      PassedArguments[Index] = Value;
    }
  }

  // If we didn't get a script, just take the generated value.
  for (int i = 0; i < PassedArguments.size(); i++) {
    if (PassedArguments[i] == "") {
      PassedArguments[i] =
          "std::get<" + std::to_string(i) + ">(GeneratedValues)";
    }
  }

  ToReturn =
      ReplaceSymbolAndLog(std::cout, ToReturn, "GENERATOR_SCRIPT_LAMBDAS",
                          JoinVectorOfStrings(GeneratorScriptLambdas, ";\n"));

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn,
                                 "DETERMINE_PASSED_FROM_GENERATED",
                                 JoinVectorOfStrings(PassedArguments, ", "));

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "TEST_CONDITION",
                                 ToWrite.PredicateSource);

  // TODO: These messages should be much clearer and more informative,
  // especially if they're going to sit in a log file thats 10M long
  ToReturn = ReplaceSymbolAndLog(
      std::cout, ToReturn, "TEST_BODY_PASS",
      "std::cout << \"Test iteration passed\" << std::endl;\n");

  ToReturn = ReplaceSymbolAndLog(std::cout, ToReturn, "TEST_BODY_FAIL",
                                 "EverythingPassed = false;\n\tstd::cout << \" "
                                 "Test iteration failed\" << std::endl;\n");

  WritingLogging.DecreaseIndentationLevel();
  WritingLogging.Log(std::cout, true, "Finished writing UnmaskedPredicateTest");
  return {ToReturn, TestCreationStatus::ALL_OK};
}
