UNMASKEDCXX README
------------------

UnmaskedCXX is a WIP automatic test generator for C/C++, written in C++. Currently the project is only tested under C++17, but is hoping to target everything from C++11 onwards.

./SampleCXXProject/ gives an example of an extremely small (header-only) library for which UnmaskedCXX can generate tests.

Build Instructions
------------------

To build the software itself, use CMake.

From the root directory of this project, run
`cmake .`

And two targets will become available for your build system.

The first is `TestAuthor`, which you will need to build first.
On Linux/Mac, you can run

`make TestAuthor`
`./TestAuthor`

to build and run the `TestAuthor`. Currently this will only search for `SampleCXXProject/Calculations.h` by name, however in future this will be generalised.

Then run
`make RunTests`
`./UnmaskedCreatedTests/RunTests`

To actually run the generated tests. The results will be fed to the standard output.

To save the results to a file, use a Unix pipeline. E.g.

`./UnmaskedCreatedTests/RunTests > Results.txt`
