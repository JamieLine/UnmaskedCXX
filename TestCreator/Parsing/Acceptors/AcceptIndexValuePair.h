#ifndef TESTCREATOR_ACCEPTORS_ACCEPTINDEXVALUEPAIR_H
#define TESTCREATOR_ACCEPTORS_ACCEPTINDEXVALUEPAIR_H

#include <string>
#include <utility>

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptIndexValuePair(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::pair<std::string, std::string>>;

#endif /* TESTCREATOR_ACCEPTORS_ACCEPTINDEXVALUEPAIR_H */
