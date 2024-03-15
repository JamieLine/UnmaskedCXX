#ifndef TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H
#define TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H

#include "TestCreator/Structs/GeneratorSettingBunch.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

auto AcceptGeneratorSettings(TokenArray::iterator& FirstToken)
    -> ParsedResult<GeneratorSettingBunch>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTGENERATORSETTINGS_H */
