#ifndef TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSTABILISINGSETTEST_H
#define TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSTABILISINGSETTEST_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/ParsedUnmaskedPredicateTest.h"
#include "TestCreator/Structs/TokenArray.h"

// This intentionally returns a different test.
// This test is effectively shorthand presented
// to the user.
auto AcceptUnmaskedStabilisingSetTest(TokenArray::iterator& FirstToken)
    -> ParsedResult<ParsedUnmaskedPredicateTest>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSTABILISINGSETTEST_H */
