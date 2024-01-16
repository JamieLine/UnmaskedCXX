cmake_minimum_required(VERSION 3.12)

#set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")
set (CMAKE_CXX_FLAGS "-std=c++17")

SET(TESTED_PROJECT_INCLUDE_DIRECTORY "SampleCXXProject")

SET(UNMASKED_USER_INCLUDE_DIRECTORY "UserlandIncludes")

project(UnmaskedCXX VERSION 1.0 LANGUAGES CXX)

SET(TEST_AUTHOR_FILES "TestAuthors/CreateStabilisingSetTest.cpp")
SET(UNMASKED_INCLUDE_DIRECTORIES "./")
SET(EVERYWHERE_INCLUDED_SOURCE_CODE "StringOperations.cpp" "Logging.cpp")

add_executable(TestRunner TestAuthors/RunAllTests.cpp ${EVERYWHERE_INCLUDED_SOURCE_CODE} ${TEST_AUTHOR_FILES})
target_include_directories(TestRunner PUBLIC ${TESTED_PROJECT_INCLUDE_DIRECTORY})
target_include_directories(TestRunner PUBLIC ${UNMASKED_INCLUDE_DIRECTORIES})
target_include_directories(TestRunner PUBLIC ${UNMASKED_USER_INCLUDE_DIRECTORY})

add_executable(TestAuthor TestAuthors/ScanForAllTests.cpp ${EVERYWHERE_INCLUDED_SOURCE_CODE} ${TEST_AUTHOR_FILES})
target_include_directories(TestAuthor PUBLIC ${TESTED_PROJECT_INCLUDE_DIRECTORY})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_INCLUDE_DIRECTORIES})
target_include_directories(TestAuthor PUBLIC ${UNMASKED_USER_INCLUDE_DIRECTORY})