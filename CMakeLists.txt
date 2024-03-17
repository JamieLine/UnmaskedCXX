cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_FLAGS "-std=c++11")

SET(TESTED_PROJECT_INCLUDE_DIRECTORY "SampleCXXProject")

SET(UNMASKED_USER_INCLUDE_DIRECTORY "UserlandIncludes")

project(UnmaskedCXX VERSION 1.0 LANGUAGES CXX)

SET(GENERATORS "Generators/GeneratorParameterStore.cpp")
SET(TEST_CREATOR_PARSING_ACCEPTORS "TestCreator/Parsing/Acceptors/AcceptAnyToken.cpp"
           "TestCreator/Parsing/Acceptors/AcceptIndexAsString.cpp"
           "TestCreator/Parsing/Acceptors/AcceptIndexValuePair.cpp"
           "TestCreator/Parsing/Acceptors/AcceptSpecificString.cpp")

SET(TEST_CREATOR_PARSING_MICROPARSERS "TestCreator/Parsing/MicroParsers/AcceptGeneratorSettings.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptLambda.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptSTDFunction.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptType.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptUnmaskedPredicateTest.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptUnmaskedSetParameter.cpp"
           "TestCreator/Parsing/MicroParsers/AcceptUnmaskedStabilisingSetTest.cpp"
           "TestCreator/Parsing/MicroParsers/BracketAcceptor.cpp")

# Intentionally do not include TestCreator/Parsing/Parsers as that is old code and probably going to be removed entirely

SET(TEST_CREATOR_STRUCTS "TestCreator/Structs/Filepath.cpp"
       "TestCreator/Structs/GeneratorParameterStoreSeed.cpp"
       "TestCreator/Structs/TestCreationStatus.cpp"
       "TestCreator/Structs/GeneratorSettingBunch.cpp")

SET(TEST_CREATOR_WRITING_FRAGMENT_WRITERS "TestCreator/Writing/FragmentWriters/WriteForLoop.cpp")
SET(TEST_CREATOR_WRITING_TEST_WRITERS "TestCreator/Writing/TestWriters/WriteUnmaskedPredicateTest.cpp")

SET(TEST_DRIVER "TestCreator/Driver/Driver.cpp")

#SET(TEST_AUTHOR_FILES "TestCreator/Parsing/Parsers/CreateStabilisingSetTest.cpp"
#					  "TestCreator/Parsing/Parsers/CreateAlwaysReturnValueTest.cpp"
#					  "TestCreator/Structs/GeneratorParameterStoreSeed.cpp"
#					  "TestCreator/Structs/TestCreationStatus.cpp"
#					  "TestCreator/TestCreator.cpp"
#					  "TestCreator/Structs/Filepath.cpp"
#					  "TestCreator/Parsing/Parsers/CreateAdditionalIncludeLine.cpp")
SET(UNMASKED_INCLUDE_DIRECTORIES "./")
SET(EVERYWHERE_INCLUDED_SOURCE_CODE "Logging.cpp" "SetOperations.cpp")

# CMake complains if it is asked to create build information for non-existent files.
if(EXISTS "UnmaskedCreatedTests/TestRunner.cpp")
  add_executable(RunTests UnmaskedCreatedTests/TestRunner.cpp Generators/GeneratorParameterStore.cpp ${EVERYWHERE_INCLUDED_SOURCE_CODE})
  target_include_directories(RunTests PUBLIC ${TESTED_PROJECT_INCLUDE_DIRECTORY})
  target_include_directories(RunTests PUBLIC ${UNMASKED_INCLUDE_DIRECTORIES})
  target_include_directories(RunTests PUBLIC ${UNMASKED_USER_INCLUDE_DIRECTORY})
  set_target_properties( RunTests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "./UnmaskedCreatedTests"
 )
  message("TestRunner created sucessfully.")
else()
  message("WARNING: CMAKE COULD NOT FIND UnmaskedCreatedTests/TestRunner.cpp. This file will be generated once TestAuthor is run. At this point, re-run CMake and then RunTests will be available as a target.")
endif()

add_executable(TestAuthor Main.cpp
     ${TEST_DRIVER}
     ${EVERYWHERE_INCLUDED_SOURCE_CODE}
     ${TEST_AUTHOR_FILES}
     ${TEST_CREATOR_PARSING_ACCEPTORS}
     ${TEST_CREATOR_PARSING_MICROPARSERS}
     ${TEST_CREATOR_STRUCTS}
     ${TEST_CREATOR_WRITING_FRAGMENT_WRITERS}
     ${TEST_CREATOR_WRITING_TEST_WRITERS})

target_include_directories(TestAuthor PUBLIC ${TESTED_PROJECT_INCLUDE_DIRECTORY})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_INCLUDE_DIRECTORIES})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_USER_INCLUDE_DIRECTORY})
