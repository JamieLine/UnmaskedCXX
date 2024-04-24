#ifndef TESTCREATOR_PARSING_PERFORMSUBTASK_H
#define TESTCREATOR_PARSING_PERFORMSUBTASK_H

#include <string>

#include "TestCreator/Structs/Task.h"
#include "TestCreator/Structs/TokenArray.h"

template <typename ReturnType, typename... ArgumentTypesWithoutToken>
ReturnType PerformSubTask(std::string Description,
                          ReturnType (*F)(TokenArray::RawTokenIterator&,
                                          ArgumentTypesWithoutToken...),
                          TokenArray::RawTokenIterator& FirstToken,
                          ArgumentTypesWithoutToken... Args) {
  TokenArray& ThisTokenArray = FirstToken.GetArray();

  ThisTokenArray.CurrentTask->SubTasks.emplace_back(
      Description, ThisTokenArray.CurrentTask, FirstToken);
  ThisTokenArray.CurrentTask = &ThisTokenArray.CurrentTask->SubTasks.back();

  ReturnType ReturnValue = F(FirstToken, Args...);

  ThisTokenArray.CurrentTask = ThisTokenArray.CurrentTask->Parent;

  return ReturnValue;
}

#endif /* TESTCREATOR_PARSING_PERFORMSUBTASK_H */
