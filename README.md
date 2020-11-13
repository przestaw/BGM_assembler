# BGM-asm prototype
## Binary Genome Map Assembly

### Program Usage

##### Options:

| command               | type                  | description |
| :---                  | :----:                | :---  |
| -h [ --help ]         | flag                  | Help screen |
| -i [ --input ]        | arg                   | Input file |
| -o [ --output ]       | arg                   | Output file |
| --margin_quant        | arg                   | Margin of maps overlap in quants [min no of symbols for overlap to be meaningful] |
| --margin_part         | arg                   | Margin of maps part overlap[min part of map overlap to be meaningful] |
| --thresh              | arg                   | Max part of differences between two maps to be considered meaningful |
| -t [ --threads ]      | arg  (default = 1)    | No of threads used for parallel algorithm parts[0 will force to use max detected threads, higher values will create desired threads count] |
| -f [ --folds ]        | arg  (default = 1)    | No of folds [repeated assemblies] |
| --marker_preference   | arg  (default = 1)    | preference of marker (true) or empty place (false) during consensus when 50% of maps has 1 and 50% has 0 |
| --reuse_maps          | flag                  | Reuse rmaps with contigs [from at least 2 rmaps] in each fold |
| -v [ --verbose ]      | flag                  | Verbose output [current state] |



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

    - **BGM-asm-library** - library containing all classes excluding parameters
    - **BGM-asm-program** - program available for command line usage
    - **BGM-asm-test** - unit tests for Cpp library made with *boost*
    - **BGM-asm-check** - dummy target for test run during library compilation
    - **BGM-asm-python** - python package utilizing boost.python
    - **BGM-asm-python-test** - unit tests for python package made with *boost*
    - **BGM-asm-python-check** - dummy target for test run during python package compilation
