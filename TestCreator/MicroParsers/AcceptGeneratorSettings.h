#ifndef TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H
#define TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H

#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptGeneratorSettings(TokenArray::iterator& FirstToken)
    -> ParsedResult<std::string>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H */
