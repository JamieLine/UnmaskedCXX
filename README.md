<h1 style="text-align: center;" align="center">UnmaskedCXX Readme</h3>

<p align="center">
<img src="https://github.com/JamieLine/UnmaskedCXX/blob/main/Graphics/PurpleLogo.png?raw=true" width=40% height=40% align="center">
</p>

About
-----

UnmaskedCXX is a WIP automatic test generator for C/C++, written in C++. Currently the project is only tested under C++17, but is hoping to target everything from C++11 onwards. 

`SampleCXXProject/` gives an example of an extremely small (header-only) library for which UnmaskedCXX can generate tests.

How to Build and Run
------------------

To build the software itself, use CMake.

From the root directory of this project, run
`cmake .`

And two targets will become availabBuild and Run Instructionsle for your build system.

The first is `TestAuthor`, which you will need to build first.
On Linux/Mac, you can run

`make TestAuthor`

`./TestAuthor`

to build and run the `TestAuthor`. Currently this will only search for `SampleCXXProject/Calculations.h` by name, however in future this will be generalised.

Then run
`make RunTests`

`./UnmaskedCreatedTests/RunTests`

To actually run the generated tests. The results will be fed to a file in the UnmaskedCXX root directory called `Report.csv`.

If there is a problem writing any content to any file, that content will be written into the standard output to allow for easy data recovery.

Usage Instructions
------------------

See `SampleCXXProject/Calculations.h` for an example.


In a source file which contains the implementation of a function you wish to test, create a new function which will never be called directly.
In `SampleCXXProject/Calculations.h`, this function is `void TestSpecifications()`, but the actual name does not matter. This function should not
appear in any compiled binaries and will never be called. 

Within this new function, add the specifications of the tests you wish to run. These will take the following form.

`UnmaskedTEST_TYPE(std::function<RETURN_TYPE(ARGUMENT_TYPES)>, PARAMETERS_TO_THE_TEST);`

For example, to run a Stabilising Set Test on a function `int AddInts(int A, int B)`, where the argument `A` is fixed to the value `0` the test declaration will be the following.

`UnmaskedStabilisingSetTest(std::function<int(int, int)>, 0, 0)`.


