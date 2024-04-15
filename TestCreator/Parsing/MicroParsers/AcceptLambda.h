#ifndef TESTCREATOR_MICROPARSERS_ACCEPTLAMBDA_H
#define TESTCREATOR_MICROPARSERS_ACCEPTLAMBDA_H

#include <set>

#include "TestCreator/Parsing/MicroParsers/BracketAcceptor.h"
#include "TestCreator/Structs/ParsedResult.h"
#include "TestCreator/Structs/TokenArray.h"

const std::set<std::pair<std::string, std::string>> LegalParameters = {
    {"int", "ReturnedValue"},
};

auto GetCaptureAllLegalParametersSource() -> std::string;

auto AcceptLambda(TokenArray::RawTokenIterator& FirstToken)
    -> ParsedResult<std::string>;

#endif /* TESTCREATOR_MICROPARSERS_ACCEPTLAMBDA_H */
