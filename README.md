Game engine project for Systems Programming C++

# Prerequisites
- CMake 3.10 or higher
- A C++ compiler that supports C++17
- GoogleTest library installed on your system (only if you want to run the tests)
- Libpulse development installed (sudo apt-get install libpulse-dev)
- Gcov installed on your system if you want the gcov report (sudo apt-get install gcovr)
- Freetype installed (sudo apt-get install libfreetype-dev)

# How to Download and Install

To download and install the library, follow these steps:

1. Clone the repository:
```sh
    git clone https://github.com/Engine-BRAVO/Engine-BRAVO.git
```

2. Update the submodules
```sh
    git submodule update --init --recursive --depth 1
```

3. Navigate to the build directory:
```sh
    cd Engine-BRAVO/code/build
```

4. Run CMake to configure the project
```sh
    cmake ..
```

5. Build the project using `make` or `cmake` with the appropriate number of jobs:
```sh
    make -j$(nproc)
```
```sh
    cmake --build . -- -j$(nproc)
```

# How to Build with Debug Options

To build the project with debug options enabled, follow these steps:

1. Run CMake to configure the project with debug options:
```sh
    cmake .. -DENABLE_DEBUG=ON
```

2. Build the project using `make` or `cmake` with the appropriate number of jobs:
```sh
    make -j$(nproc)
```
```sh
    cmake --build . -- -j$(nproc)
```

# How to Run the Program with Valgrind
To run the program with valgrind, use the following command:
```sh
    cmake --build . --target run_valgrind -- -j$(nproc)
```

# How to Build and Run the Tests

1. Configure the project with tests enabled by setting `ENABLE_TESTS=ON`:
    ```sh
    cmake .. -DENABLE_TESTS=ON
    ```

2. Build and run the tests:
    ```sh
    cmake --build . --target run_tests -- -j$(nproc)
    ```

Note: GoogleTest and GoogleMock libraries must be installed on your system to build the tests.

# How to Install Google Mock

1. Go to the downloads folder:
```sh
    cd ~/Downloads
```

2. Clone the Google Test repository:
```sh
    git clone https://github.com/google/googletest.git
    cd googletest
```

3. Create a build directory:
```sh
    mkdir build
    cd build
```

4. Configure and build:
```sh
    cmake ..
    make -j$(nproc)
```

5. Install:
```sh
    sudo make install
```

# How to generate Doxygen documentation
1. Go to the build folder:
```sh
    cd code/build
```

2. Run the Doxygen target
```sh
    cmake --build . --target Doxygen
```

The Doxygen output will be placed under code/build/doxygen/html. Open the index.html file to go to docs' mainpage.
