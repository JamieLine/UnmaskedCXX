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

And the first target will become available for your build system.

That target is `TestAuthor`, which you will need to build first.

On Linux/Mac, you can run

`make TestAuthor`

`./TestAuthor`

to build and run the `TestAuthor`. Currently this will only search for `SampleCXXProject/Calculations.h` by name, however in future this will be generalised.

Then re-run `cmake .` and it will generate the second target for you. The new target is `RunTests`, which by default outputs its binaries to `UnmaskedCreatedTests/`.

Then run
`make RunTests`

`./UnmaskedCreatedTests/RunTests`

To actually run the generated tests. The results will be fed to a file in the UnmaskedCXX root directory called `Report.csv`.

If there is a problem writing any content to any file, that content will be written into the standard output to allow for easy data recovery.

Usage Instructions
------------------

See `SampleCXXProject/Calculations.h` for an example.


In a source file which contains the implementation of a function you wish to test, include `UserlandIncludes/UnmaskedTests.h` and create a new function which will never be called directly. In `SampleCXXProject/Calculations.h`, this function is `void TestSpecifications()`, but the actual name does not matter. This function should not
appear in any compiled binaries and will never be called. 

Within this new function, add the specifications of the tests you wish to run. These will take the following form.

`UnmaskedTEST_TYPE(std::function<RETURN_TYPE(ARGUMENT_TYPES)>, PARAMETERS_TO_THE_TEST);`

For example, to run a Stabilising Set Test on a function `int AddInts(int A, int B)`, where the argument `A` is fixed to the value `0` the test declaration will be the following.

`UnmaskedStabilisingSetTest(std::function<int(int, int)>, 0, 0)`.

License
-------

Both object and source representations of this software are licensed under CC-BY-NC 4.0. See `LICENSE` for the full text of the license, and https://creativecommons.org/licenses/by-nc/4.0/ for a summary of its terms. 

For the avoidance of doubt, UnmaskedCXX does not consider a commercial entity running the software in a manner which does not directly produce value, for the purposes of assessment of the software or its Author(s) as "primarily intended for or directed towards commercial advantage or monetary compensation" as stated in Section 1(i) of the license, even when the assessment part of a commercial process. We intend for users to be able to run and utilise the software in ways which do not produce value as part of an investigatory process, even if the end result of that investigation has intent to produce commercial advantage or monetary compensation.

If you wish to use UnmaskedCXX under the terms of another license, please e-mail Jamie Line (`jamietline@gmail.com`) to discuss.

Contributing
------------

At this time, UnmaskedCXX is not actively looking for contributors.