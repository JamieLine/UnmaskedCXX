#ifndef TESTCREATOR_MICROPARSERS_ACCEPTSTDFUNCTION_H
#define TESTCREATOR_MICROPARSERS_ACCEPTSTDFUNCTION_H

#include "TestCreator/Structs/ParsedFunction.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptSTDFunction(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<ParsedFunction>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTSTDFUNCTION_H */
