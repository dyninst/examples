# Example usage of [Dyninst](https://github.com/dyninst/dyninst)

## Building

Before building any of these examples, you need an existing build of Dyninst (see the Dyninst [wiki](https://github.com/dyninst/dyninst/wiki/Building-Dyninst) for details).

To configure the build, you can use

    cmake . -DDyninst_DIR=path/to/Dyninst/install/lib/cmake/Dyninst

NOTE: The last three parts of the `Dyninst_DIR` path *must* be the `lib/cmake/Dyninst` directory under your Dyninst installation.

The project can be built out-of-source using the usual `-B` and `-H` CMake flags.

The default CMake configure uses GNU Makefiles as the generator, so to build is just

    make install

To build only specific examples, you can specify their names like so

    make codeCoverage CFGraph

## Running

Each example is built in its own subdirectory under the main build directory (if you didn't specify `-B` when running CMake, the build directory is just the root of the `examples` project directory). For example, to run the codeCoverage example

	$ cd codeCoverage
	$ export DYNINSTAPI_RT_LIB=path/to/dyninst/lib/libdyninstAPI_RT.so
	$ export LD_LIBRARY_PATH=.:path/to/dyninst/lib:$LD_LIBRARY_PATH
	$ ./code_coverage -p testcc testcc.inst
	$ ./testcc.inst
