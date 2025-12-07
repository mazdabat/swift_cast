# swift_cast

A signage application built with SDL3. This project will continue to evolve with additional digital signage features.

_NOTE_: The project currently runs only on Linux and requires GCC 10 or newer.

## Prerequisites

- GCC supporting C++ standard 17 or later
- CMake 3.31+
- SDL 3

## Build

1. Fetch _SDL3_ source code

   `git submodule update --init --recursive`

2. Create _build_ directory in the project root

   `mkdir build`

3. Navigate to _build_ directory

   `cd build`

4. Build the project using _cmake_

   `cmake --build . --target=all`

## Run the applicaiton

`./bin/swift-cast`
