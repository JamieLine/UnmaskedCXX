#ifndef TESTCREATOR_PARSING_MICROPARSERS_ACCEPTIDENTIFIERVALUEPAIR_H
#define TESTCREATOR_PARSING_MICROPARSERS_ACCEPTIDENTIFIERVALUEPAIR_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptIdentifierValuePair(TokenArray::RawTokenIterator& FirstToken) -> ParsedResult<std::pair<std::string, std::string>>;

#endif /* TESTCREATOR_PARSING_MICROPARSERS_ACCEPTIDENTIFIERVALUEPAIR_H */
