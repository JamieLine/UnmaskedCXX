#include "WriteForLoop.h"

#include "StringOperations.h"
#include "TestCreator/Structs/LegalIdentifier.h"

auto WriteForLoop(TestCreationContext& Context, std::size_t NumberOfIterations,
                  std::string LoopBody) -> std::string {
  return WriteForLoop(Context, NumberOfIterations, LegalIdentifier("i"),
                      LoopBody);
}

auto WriteForLoop(TestCreationContext& Context, std::size_t NumberOfIterations,
                  LegalIdentifier LoopVariableIdentifier, std::string LoopBody)
    -> std::string {
  const std::string& LoopVar = LoopVariableIdentifier.ID;

  std::string ToReturn = "for (int " + LoopVar + " = 0; " + LoopVar + "< " +
                         std::to_string(NumberOfIterations) + "; " + LoopVar +
                         "++) {";

  Context.CurrentIndentationLevel += 1;

  LoopBody = IndentEveryLine(LoopBody, Context.CurrentIndentationLevel);

  Context.CurrentIndentationLevel -= 1;

  ToReturn += "}\n";

  return ToReturn;
}