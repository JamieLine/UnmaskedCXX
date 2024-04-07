#ifndef TESTCREATOR_WRITING_FRAGMENTWRITERS_WRITEFORLOOP
#define TESTCREATOR_WRITING_FRAGMENTWRITERS_WRITEFORLOOP

#include "TestCreator/Structs/LegalIdentifier.h"
#include "TestCreator/Structs/TestCreationContext.h"

auto WriteForLoop(TestCreationContext& Context, std::size_t NumberOfIterations,
                  std::string LoopBody) -> std::string;
auto WriteForLoop(TestCreationContext& Context, std::size_t NumberOfIterations,
                  LegalIdentifier LoopVariableIdentifier, std::string LoopBody)
    -> std::string;

#endif /* TESTCREATOR_WRITING_FRAGMENTWRITERS_WRITEFORLOOP */
