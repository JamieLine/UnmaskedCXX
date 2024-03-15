#ifndef TESTCREATOR_ACCEPTORS_ACCEPTSPECIFICSTRING_H
#define TESTCREATOR_ACCEPTORS_ACCEPTSPECIFICSTRING_H

#include <string>
#include <utility>

#include "../../Structs/ParsedResult.h"
#include "../../Structs/TokenArray.h"

auto AcceptSpecificString(TokenArray::iterator& FirstToken,
                          std::string TargetString) -> bool;

#endif /* TESTCREATOR_ACCEPTORS_ACCEPTSPECIFICSTRING_H */
