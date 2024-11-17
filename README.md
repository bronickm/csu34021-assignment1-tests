# CSU34021 Computer Architecture II (2024/25) Assignment 1 Tests
This repository contains the testing setup used for evaluating the submissions for Assignment 1 for the CSU34021 Computer Architecture II module (2024/25).

The assembly implementations to be tested should be placed in `task1.asm` and `task2.asm` files, respectively. The `Task1Run.ps1` and `Task2Run.ps1` scripts can then be used to compile the test program, assemble the provided implementation, link, and execute the tests. Note that the scripts do not recompile the test C++ programs with every execution.

Running the program requires Visual Studio with the "Desktop development with C++" workload to be installed. The paths specified in `VcWrapper.bat` and `VcWrapper64.bat` must point to the corresponding `vcvars` files shipped with Visual Studio.
