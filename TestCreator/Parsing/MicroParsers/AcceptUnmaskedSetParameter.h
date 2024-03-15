#ifndef TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSETPARAMETER
#define TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSETPARAMETER

#include <string>
#include <utility>

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptUnmaskedSetParameter(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::pair<std::string, std::string>>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTUNMASKEDSETPARAMETER */
