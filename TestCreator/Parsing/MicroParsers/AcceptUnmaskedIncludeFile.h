#ifndef TESTCREATOR_PARSING_MICROPARSERS_ACCEPTUNMASKEDINCLUDEFILE
#define TESTCREATOR_PARSING_MICROPARSERS_ACCEPTUNMASKEDINCLUDEFILE

#include "TestCreator/Structs/Filepath.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TestCreationStatus.h"
#include "TestCreator/Structs/TokenArray.h"

ParsedResult<std::string> AcceptUnmaskedIncludeFile(
    TokenArray::RawTokenIterator& FirstToken);

#endif /* TESTCREATOR_PARSING_MICROPARSERS_ACCEPTUNMASKEDINCLUDEFILE */
