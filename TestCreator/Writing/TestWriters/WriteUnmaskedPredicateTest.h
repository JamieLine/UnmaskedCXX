#ifndef TESTCREATOR_RESULTWRITERS_WRITEUNMASKEDPREDICATETEST
#define TESTCREATOR_RESULTWRITERS_WRITEUNMASKEDPREDICATETEST

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TestCreationContext.h"
#include "TestCreator/Structs/TestCreationStatus.h"

auto WriteUnmaskedPredicateTest(const TestCreationContext& Context,
                                const ParsedUnmaskedPredicateTest& ToWrite)
    -> WithStatus<std::string>;

#endif /* TESTCREATOR_RESULTWRITERS_WRITEUNMASKEDPREDICATETEST */
