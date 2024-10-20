# COMP2811 Coursework 1

## Provided Files

These files are to be edited by you:

* `quake.hpp`, `quake.cpp` - Definition and implementation of `Quake` class
  (mostly complete).

* `dataset.hpp`, `dataset.cpp` - Definition and implementation of
  `QuakeDataset` class (mostly incomplete).

* `main.cpp` - The `quaketool` application (currently empty).

The following are supporting files that should *not* be edited:

* `CMakeLists.txt` - CMake build file. You won't need to change this unless
  you decide to use different libraries for argument parsing or I/O.

* `test_quake.cpp`, `test_dataset.cpp` - Tests for the `Quake` and
  `QuakeDataset` classes.

* `doctest.h` - Header file providing the testing framework.

* `argparse.hpp` - Command line argument parsing library.

* `csv.hpp` - Library for reading CSV files.

* `tabulate.hpp` - Library for displaying tables in the console.

You can use different libraries for argument parsing, CSV file input and
tabular display if you like, but we recommend these ones as being reasonably
easy to use.

The `data` subdirectory contains sample data files -- see the README in that
directory for further details.

## Build Instructions

To set up the build environment, do

    mkdir build && cd build && cmake ..

To build the tests for the `Quake` class, do

    make test_quake

To build the tests for `QuakeTest`, do

    make test_dataset

To build the `quaketool` application, do

    make quaketool

All of these commands should be executed in the `build` subdirectory.

To run the tests for the `Quake` class, enter

    ./test_quake

The other tests and the application are run in a similar manner.
