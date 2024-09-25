# Engine-BRAVO
Game engine project for Systems Programming C++
## How to Download and Install

To download and install the library, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/Engine-BRAVO/Engine-BRAVO.git
    ```

2. Update the submodules
    ```sh
    git submodule update --init --recursive
    ```

3. Navigate to the build directory:
    ```sh
    cd Engine-BRAVO/build
    ```

4. Run CMake to configure the project. If you want to include Google Test (gtest), use the following command:
    ```sh
    cmake -DENABLE_GTEST=ON ..
    ```
    Otherwise, simply run:
    ```sh
    cmake ..
    ```

5. Build the project using `make` or `cmake` with the appropriate number of jobs:
    ```sh
    make -j$(nproc)
    ```
    or
    ```sh
    cmake --build . -- -j$(nproc)
    ```