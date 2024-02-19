#ifndef TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDPREDICATETEST_H
#define TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDPREDICATETEST_H

#include <string>
#include <vector>

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptUnmaskedPredicateTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDPREDICATETEST_H */
