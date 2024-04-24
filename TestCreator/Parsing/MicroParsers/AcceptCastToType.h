#ifndef TESTCREATOR_PARSING_ACCEPTORS_ACCEPTCASTTOTYPE_H
#define TESTCREATOR_PARSING_ACCEPTORS_ACCEPTCASTTOTYPE_H

#include <string>

#include "TestCreator/Structs/TokenArray.h"

auto AcceptCastToType(TokenArray::RawTokenIterator& FirstToken, std::string TypeName)
    -> bool;

#endif /* TESTCREATOR_PARSING_ACCEPTORS_ACCEPTCASTTOTYPE_H */
