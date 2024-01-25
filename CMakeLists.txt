cmake_minimum_required(VERSION 3.12)

set (CMAKE_CXX_FLAGS "-std=c++17")

SET(TESTED_PROJECT_INCLUDE_DIRECTORY "SampleCXXProject")

SET(UNMASKED_USER_INCLUDE_DIRECTORY "UserlandIncludes")

project(UnmaskedCXX VERSION 1.0 LANGUAGES CXX)

SET(TEST_AUTHOR_FILES "TestAuthors/CreateStabilisingSetTest.cpp" "TestAuthors/CreateAlwaysReturnValueTest.cpp")
SET(UNMASKED_INCLUDE_DIRECTORIES "./")
SET(EVERYWHERE_INCLUDED_SOURCE_CODE "StringOperations.cpp" "Logging.cpp")

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

add_executable(TestAuthor TestAuthors/ScanForAllTests.cpp ${EVERYWHERE_INCLUDED_SOURCE_CODE} ${TEST_AUTHOR_FILES})
target_include_directories(TestAuthor PUBLIC ${TESTED_PROJECT_INCLUDE_DIRECTORY})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_INCLUDE_DIRECTORIES})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_USER_INCLUDE_DIRECTORY})