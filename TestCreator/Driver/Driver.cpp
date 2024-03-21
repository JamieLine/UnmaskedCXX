#include "Driver.h"

#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "Logging.h"
#include "StringOperations.h"
#include "TestCreator/Parsing/MicroParsers/AcceptUnmaskedPredicateTest.h"
#include "TestCreator/Parsing/MicroParsers/AcceptUnmaskedSetParameter.h"
#include "TestCreator/Parsing/MicroParsers/AcceptUnmaskedStabilisingSetTest.h"
#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TestCreationContext.h"
#include "TestCreator/Structs/TestCreationStatus.h"
#include "TestCreator/Structs/TokenArray.h"
#include "TestCreator/Writing/TestWriters/WriteUnmaskedPredicateTest.h"
#include "VectorOperations.h"

std::vector<std::pair<TestCreationContext, ParsedUnmaskedPredicateTest>>
    Driver::StoredPredicateTests;

std::vector<std::pair<TestCreationContext, ParsedUnmaskedPredicateTest>>
    Driver::StoredStabilisingSetTests;

std::vector<Filepath> Driver::GeneratedSourceFilepaths;

auto Driver::ParseInputFile(const Filepath& FileAddress) -> TestCreationStatus {
  Log(std::cout, LOG, "Called ParseInputFile on");
  Log(std::cout, VALUE_OUTPUT, FileAddress.Path);

  Optional<std::string> MaybeInputFile = FileAddress.ReadContentsOfFile();
  if (!MaybeInputFile.DataExists) {
    Log(std::cout, ERROR, "Attempted to read file which cannot be opened.");
    Log(std::cout, VALUE_OUTPUT, FileAddress.Path);
    return TestCreationStatus::COULD_NOT_READ_INPUT_FILE;
  }

  std::string InputFile = MaybeInputFile.Data;

  // We wish to tokenize this file but some delimiters are important and some
  // can be discarded. Broadly, it is important if it carries specific meaning.
  const std::vector<std::string> KeptDelimiters = {
      "&", ".", ",", ";", "(", ")", "{", "}", "[", "]", "<", ">", "\""};
  const std::vector<std::string> DiscardedDelimiters = {
      " ",
      "\n",
      "\t",
  };

  TestCreationContext CurrentContext(FileAddress);
  std::cout << "THIS IS THE CURRENT CONTEXT FILE ADDRESS";
  std::cout << FileAddress.Path + "\n";
  std::cout << CurrentContext.TestDefinitionPath.Path;
  std::cout << std::endl;

  TokenArray Tokens = Tokenize(InputFile, KeptDelimiters, DiscardedDelimiters);

  TokenArray::iterator CurrentToken = Tokens.begin();

  // THE OLD VERSION OF THIS USED A MAP TO FIND NEARBY TOKENS WHICH MAY BE WAY
  // FASTER? OR MAYBE SLOWER?
  while (CurrentToken != Tokens.end()) {
    // This really wants to be a switch statement but those are not allowed on
    // strings

    if (*CurrentToken == "UnmaskedPredicateTest") {
      auto ParsedTest = AcceptUnmaskedPredicateTest(CurrentToken);
      if (!ParsedTest.WasLegalInput) {
        Log(std::cout, ERROR,
            "Attempted to parse UnmaskedPredicateTest, but it was illegal.");
        return TestCreationStatus::ATTEMPTED_ILLEGAL_PARSE;
      }

      CurrentContext.GeneratedFunctionName =
          "UnmaskedCreatedTests_PredicateTest_" +
          std::to_string(CurrentContext.CurrentTestNumber) + "_" +
          ParsedTest.Result.TestedFunction.Name;

      StoredPredicateTests.emplace_back(CurrentContext, ParsedTest.Result);
      CurrentContext.CurrentTestNumber++;
    }

    else if (*CurrentToken == "UnmaskedStabilisingSetTest") {
      auto ParsedTest = AcceptUnmaskedStabilisingSetTest(CurrentToken);
      if (!ParsedTest.WasLegalInput) {
        Log(std::cout, ERROR,
            "Attempted to parse UnmaskedStabilisingSetTest, but it was "
            "illegal.");

        PrintAround(CurrentToken, Tokens);
        return TestCreationStatus::ATTEMPTED_ILLEGAL_PARSE;
      }

      CurrentContext.GeneratedFunctionName =
          "UnmaskedCreatedTests_StabilisingSetTest_" +
          std::to_string(CurrentContext.CurrentTestNumber) + "_" +
          ParsedTest.Result.TestedFunction.Name;

      StoredStabilisingSetTests.emplace_back(CurrentContext, ParsedTest.Result);

      CurrentContext.CurrentTestNumber++;
    }

    else if (*CurrentToken == "UnmaskedSetParameter") {
      auto ParsedParameter = AcceptUnmaskedSetParameter(CurrentToken);
      if (!ParsedParameter.WasLegalInput) {
        Log(std::cout, ERROR,
            "Attempted to parse UnmaskedSetParameter, but it was illegal.");
        return TestCreationStatus::ATTEMPTED_ILLEGAL_PARSE;
      }

      CurrentContext.Params.ParametersAndValues.emplace_back(
          ParsedParameter.Result);
    }

    // TODO: THERES A FEW Unmasked*** THAT CANT BE ACCEPTED YET

    else {
      CurrentToken++;
    }
  }

  return TestCreationStatus::ALL_OK;
}

auto Driver::WriteAllStoredInputs() -> TestCreationStatus {
  const std::string RootDir = "./UnmaskedCreatedTests/";

  for (auto& PredicateTestPair : StoredPredicateTests) {
    TestCreationContext& Context = PredicateTestPair.first;
    ParsedUnmaskedPredicateTest& Test = PredicateTestPair.second;

    std::cout << "WHEN WRITING, THIS IS THE CONTEXT PATH\n";
    std::cout << Context.TestDefinitionPath.Path;
    std::cout << std::endl;

    WithStatus<std::string> Result = WriteUnmaskedPredicateTest(Context, Test);

    if (Result.Status != TestCreationStatus::ALL_OK) {
      Log(std::cout, ERROR,
          "Attempted to write UnmaskedPredicateTest, but it was illegal.");
      Log(std::cout, VALUE_OUTPUT, std::to_string(int(Result.Status)));
      return Result.Status;
    }

    Filepath OutputPath(RootDir + Context.GeneratedFunctionName + "_" +
                        std::to_string(Context.CurrentTestNumber) + ".cpp");

    OutputPath.WriteStringIntoFileOverwriting(Result.Item);
  }

  for (auto& StabilisingTestPair : StoredStabilisingSetTests) {
    TestCreationContext& Context = StabilisingTestPair.first;
    ParsedUnmaskedPredicateTest& Test = StabilisingTestPair.second;

    WithStatus<std::string> Result = WriteUnmaskedPredicateTest(Context, Test);

    if (Result.Status != TestCreationStatus::ALL_OK) {
      Log(std::cout, ERROR,
          "Attempted to write UnmaskedStabilisingSetTest, but it was illegal.");
      Log(std::cout, VALUE_OUTPUT, std::to_string(int(Result.Status)));
      return Result.Status;
    }

    Filepath OutputPath(RootDir + Context.GeneratedFunctionName + "_" + ".cpp");

    OutputPath.WriteStringIntoFileOverwriting(Result.Item);
  }

  WriteMainDriverProgram();

  return TestCreationStatus::ALL_OK;
}

auto Driver::WriteMainDriverProgram() -> TestCreationStatus {
  const Filepath TestRunnerTemplate(
      "./Templates/TestRunnerTemplate.cpp_template");

  Optional<std::string> MaybeTestRunner =
      TestRunnerTemplate.ReadContentsOfFile();

  if (!MaybeTestRunner.DataExists) {
    Log(std::cout, ERROR, "Could not open test runner template");
    return TestCreationStatus::COULD_NOT_READ_INPUT_FILE;
  }

  std::string TestRunner = MaybeTestRunner.Data;

  std::string IncludeDirectives;

  std::vector<std::string> AllIncludeDirectives;

  std::transform(
      GeneratedSourceFilepaths.begin(), GeneratedSourceFilepaths.end(),
      std::back_inserter(AllIncludeDirectives),
      [](Filepath S) { return std::string("#include \"" + S.Path + "\"\n"); });

  IncludeDirectives =
      std::accumulate(AllIncludeDirectives.begin(), AllIncludeDirectives.end(),
                      std::string(""));

  TestRunner =
      ReplaceAllInString(TestRunner, "INCLUDE_TESTS", IncludeDirectives);

  std::string TestRunningStatements = "";

  for (const auto& PredicateTest : StoredPredicateTests) {
    auto& Context = PredicateTest.first;
    auto& Test = PredicateTest.second;
    TestRunningStatements += "TestResults.emplace_back(\"" +
                             Context.GeneratedFunctionName + "_" +
                             "PredicateTest" + "\", " + Context.Category +
                             ", \"UnmaskedPredicateTest\", " +
                             Context.GeneratedFunctionName + "());";
  }

  TestRunner = ReplaceAllInString(TestRunner, "RUN_TESTS_AND_PUSH_RESULTS",
                                  TestRunningStatements);

  Filepath OutputFile("UnmaskedCreatedTests/TestRunner.cpp");

  bool WroteToFile = OutputFile.WriteStringIntoFileOverwriting(TestRunner);

  if (!WroteToFile) {
    Log(std::cout, LOG, "Could not write TestRunner.cpp");
    return TestCreationStatus::COULD_NOT_OPEN_OUTPUT_FILE;
  }

  return TestCreationStatus::ALL_OK;
}
