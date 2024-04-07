#ifndef TESTCREATOR_WRITING_TEMPLATES
#define TESTCREATOR_WRITING_TEMPLATES

#include "TestCreator/Structs/Filepath.h"

// We turn off the linter here because it complains that Filepath initialisation
// can throw an exception But the only exception it can throw is out of memory
// and if we launch the program into an OOM error for allocating a couple of
// Filepaths it was never going to work. NOLINTBEGIN(cert-err58-cpp)

const Filepath TestSkeleton("./Templates/TestSkeleton.cpp_template");

// NOLINTEND(cert-err58-cpp)

#endif /* TESTCREATOR_WRITING_TEMPLATES */
