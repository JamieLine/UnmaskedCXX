#ifndef TESTCREATOR_STRUCTS_TASK_H
#define TESTCREATOR_STRUCTS_TASK_H

#include <string>
#include <vector>

#include "Optional.h"
#include "TokenArray.h"

struct Task {
 public:
  std::string Description;

  Optional<std::string> ErrorMessage;

  std::vector<Task> SubTasks;
  Task* Parent;

  TokenArray::RawTokenIterator StartingToken;
  TokenArray::RawTokenIterator EndingToken;

  Task(std::string InDescription, Task* InParent,
       TokenArray::RawTokenIterator InStart)
      : Description(InDescription),
        Parent(InParent),
        StartingToken(InStart),
        EndingToken(InStart),
        SubTasks(),
        ErrorMessage() {}

  void PushErrorMessage(std::string Message) {
    ErrorMessage = Optional<std::string>(Message, true);
  }

  // TODO: Add a "create child" method to use in PerformSubTask
};

#endif /* TESTCREATOR_STRUCTS_TASK_H */
