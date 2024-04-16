#ifndef ERRORREPORTER_ERRORREPORTER_H
#define ERRORREPORTER_ERRORREPORTER_H

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Optional.h"
#include "TestCreator/Structs/TokenArray.h"

struct Task {
  // This is effectively a re-branded Tree node
  // The JobDescription and Success flag are data
  // The Vector is a pointer to the children
  std::string JobDescription;
  bool Success;

  TokenArray::RawTokenIterator StartToken;
  TokenArray::RawTokenIterator EndToken;

  std::vector<Task> SubTasks;

  Task* Parent;  // Can be null, in the case of the Root

  Task(std::string InJobDescription, Task* InParent,
       TokenArray::RawTokenIterator InStartToken)
      : JobDescription(InJobDescription),
        StartToken(InStartToken),
        EndToken(InStartToken),
        SubTasks(),
        Success(false),
        Parent(InParent) {}
  // Maybe this should have a single "add child" method
};

class ErrorReporter {
 public:
  // WHEN THIS GETS MULTITHREADED
  // JUST MAKES ALL OF THESE STATIC VALUES
  // STATIC FUNCTIONS WHICH MAP A THREAD TO A VALUE
  // AND MAKE THE THREADS REGISTER THEMSELVES HERE

  static Task RootTask;

  static Task* CurrentTask;
};

template <typename ReturnType, typename... ArgumentTypesMinusTokenArrayIterator>
ReturnType TellingErrorReporter(
    std::string JobDescription,
    ReturnType (*F)(TokenArray::RawTokenIterator&,
                    ArgumentTypesMinusTokenArrayIterator...),
    TokenArray::RawTokenIterator& CurrentToken,
    ArgumentTypesMinusTokenArrayIterator... Args) {
  // Add a new child and make it the current task
  ErrorReporter::CurrentTask->SubTasks.emplace_back(
      JobDescription, ErrorReporter::CurrentTask, CurrentToken);
  ErrorReporter::CurrentTask = &ErrorReporter::CurrentTask->SubTasks.back();

  // Actually perform the task
  ReturnType ReturnValue = F(CurrentToken, Args...);

  // Re-instate the original CurrentTask, now that the sub-task is over
  ErrorReporter::CurrentTask = ErrorReporter::CurrentTask->Parent;
}

#endif /* ERRORREPORTER_ERRORREPORTER_H */
