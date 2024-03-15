#ifndef TESTCREATOR_ACCEPTORS_ACCEPTINDEXASSTRING_H
#define TESTCREATOR_ACCEPTORS_ACCEPTINDEXASSTRING_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptIndexAsString(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string>;

#endif /* TESTCREATOR_ACCEPTORS_ACCEPTINDEXASSTRING_H */
