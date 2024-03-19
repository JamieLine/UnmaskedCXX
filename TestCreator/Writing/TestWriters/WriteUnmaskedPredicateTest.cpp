#include "WriteUnmaskedPredicateTest.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "Logging.h"
#include "MapOperations.h"
#include "StringOperations.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TestCreationStatus.h"
#include "TestCreator/Writing/Templates.h"

auto WriteUnmaskedPredicateTest(const TestCreationContext& Context,
                                const ParsedUnmaskedPredicateTest& ToWrite)
    -> WithStatus<std::string> {
  auto SkeletonContent = TestSkeleton.ReadContentsOfFile();
  if (!SkeletonContent.DataExists) {
    return {"", TestCreationStatus::COULD_NOT_READ_INPUT_FILE};
  }

  std::string ToReturn = SkeletonContent.Data;

  ToReturn = ReplaceAllInString(ToReturn, "FILE_ADDRESS",
                                Context.TestDefinitionPath.Path);
  ToReturn =
      ReplaceAllInString(ToReturn, "ADDITIONAL_INCLUDES",
                         JoinVectorOfStrings(Context.AdditionalIncludes, "\n"));
  ToReturn = ReplaceAllInString(ToReturn, "TEST_FN_NAME",
                                Context.GeneratedFunctionName);
  ToReturn = ReplaceAllInString(ToReturn, "NUM_TESTS_TO_RUN",
                                std::to_string(Context.NumTestsToRun));
  ToReturn = ReplaceAllInString(
      ToReturn, "ARGUMENT_TYPES",
      JoinVectorOfStrings(ToWrite.TestedFunction.ArgumentTypes, ", "));

  // Handling the generator types is slightly more awkward, because the user may
  // specify zero or more of those types We'll work out a sensible default, and
  // if the user has passed in a non-empty replacement, we'll use that instead
  std::vector<std::string> GeneratorTypes;

  std::transform(
      ToWrite.TestedFunction.ArgumentTypes.begin(),
      ToWrite.TestedFunction.ArgumentTypes.end(),
      std::back_inserter(GeneratorTypes),
      [](std::string const& S) -> std::string { return "Generator_" + S; });

  if (!ToWrite.GeneratorSettings.Settings.empty()) {
    auto& AllGeneratorSettings = ToWrite.GeneratorSettings.Settings;

    for (auto GeneratorSettingMap : AllGeneratorSettings) {
      // Every Generator Setting must have an index
      if (!MapContainsKey(GeneratorSettingMap, std::string("Index"))) {
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      // A Generator Setting without a GeneratorType is valid, its just
      // uninteresting here
      if (!MapContainsKey(GeneratorSettingMap, std::string("GeneratorType"))) {
        continue;
      }

      // This is just a long way of saying "GeneratorTypes[Index] = GivenType"
      int Index;

      // TODO: THIS SHOULD PROBABLY BE IN A SEPARATE VALIDATION STEP
      // ALONG WITH SEVERAL OTHER VALIDATIONS AND SANITY CHECKS
      // AS SHOULD THE STUFF ABOVE
      try {
        Index = std::stoi(GeneratorSettingMap["Index"]);
      } catch (std::invalid_argument const& Exception) {
        Log(std::cout, ERROR,
            "WriteUnmaskedPredicateTest found a non-integer index.");
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      } catch (std::out_of_range const& Exception) {
        Log(std::cout, ERROR,
            "WriteUnmaskedPredicateTest found an integer but it was out of "
            "range to fit in an int.");
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      if (Index < 0 || Index >= GeneratorTypes.size()) {
        Log(std::cout, ERROR,
            "WriteUnmaskedPredicateTest was given an index which was a valid "
            "integer but outside of the bounds of the GeneratorTypes vector. "
            "Index first, GeneratorTypes size second.");
        Log(std::cout, VALUE_OUTPUT, std::to_string(Index));
        Log(std::cout, VALUE_OUTPUT, std::to_string(GeneratorTypes.size()));
        Log(std::cout, VALUE_OUTPUT,
            std::to_string(ToWrite.TestedFunction.ArgumentTypes.size()));
        return {"", TestCreationStatus::GIVEN_INVALID_PARSED_RESULT};
      }

      GeneratorTypes[Index] = GeneratorSettingMap["GeneratorType"];
    }
  }

  ToReturn = ReplaceAllInString(ToReturn, "GENERATOR_TYPES",
                                JoinVectorOfStrings(GeneratorTypes, ", "));

  // TODO: FINISH THIS

  return {ToReturn, TestCreationStatus::ALL_OK};

  // ToReturn = ReplaceAllInString(ToReturn, "GENERATOR_TYPES", )
}
