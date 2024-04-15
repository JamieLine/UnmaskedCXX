#ifndef TESTCREATOR_ACCEPTORS_ACCEPTANYTOKEN_H
#define TESTCREATOR_ACCEPTORS_ACCEPTANYTOKEN_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptAnyToken(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<std::string>;

#endif /* TESTCREATOR_ACCEPTORS_ACCEPTANYTOKEN_H */
