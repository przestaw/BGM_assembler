# OMA prototype
## Optical Map Assembly

### Program Usage


### Build instructions

- Project uses *boost* library for testing and parsing program parameters 
- When calling CMake for the first time, all needed compiler options must be
  specified on the command line.  After this initial call to CMake, the compiler
  definitions must not be included for further calls to CMake.  Other options
  can be specified on the command line multiple times including all definitions
  in the build options section below.
- Example of configuring, building, reconfiguring, rebuilding:

  ````
  # Initial configuration
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_CXX_COMPILER=clang++ ..  
    $ make
    ...
  # Second configuration
    $ make clean
    $ cmake -DCMAKE_BUILD_TYPE=Debug ..                               
    $ make
    ...
  # Third configuration
    $ rm -rf *
    $ cmake -DCMAKE_CXX_COMPILER=g++ ..        
    $ make


- CMake variables
    - **CMAKE_BUILD_TYPE** = ``Release|Debug``
      Build type can be ``Release``, ``Debug`` which chooses
      the optimization level and presence of debugging symbols.
    
    - **CMAKE_CXX_COMPILER** = <C++ compiler name>
      Specify the C++ compiler.
  
- For full documentation consult the CMake manual or execute
    ```
    cmake --help-variable VARIABLE_NAME 
  
- Project targets :

    - **build-all** - builds all available targets
    - **OMA-test** - UnitTest made with *boost*
    - **OMA-check** - dummy target for test run during library compilation
    - **OMA-library** - library containing all classes excluding parameters
    - **OMA-program** - program available for command line usage
