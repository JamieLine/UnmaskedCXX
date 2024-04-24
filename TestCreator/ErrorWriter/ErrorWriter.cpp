#include "ErrorWriter.h"

#include <iostream>

#include "TestCreator/Structs/Task.h"

void PrintTask(Task* InTask, int Indentation) {
  std::cout << std::string(Indentation, '\t');
  std::cout << InTask->Description << std::endl;

  if (InTask->SubTasks.empty()) {
    return;
  }

  for (auto Child : InTask->SubTasks) {
    PrintTask(&Child, Indentation + 1);
  }
}

void ErrorWriter::PrintEverything() {
  PrintTask(ThisArray.GetTaskTreeRoot(), 0);
}