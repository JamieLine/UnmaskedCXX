#ifndef TESTCREATOR_MICROPARSERS_ACCEPTTYPE_H
#define TESTCREATOR_MICROPARSERS_ACCEPTTYPE_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptType(TokenArray::iterator& FirstToken) -> ParsedResult<std::string>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTTYPE_H */
