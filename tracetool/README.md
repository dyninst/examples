# tracetool

## OVERVIEW
This program is called tracetool and it's an example tracing utility which
uses dyninstAPI.  The tracetool program will instrument a given program so
that function entries, exits, and callsites in the program will be
instrumented with calls into designated tracing functions in the tracetool
tracing library.

It has certain limitations  
- It only printing out the first function argument and I'm only handling function arguments and return values that are integers right now.  
- It currently only instrumenting the non library modules.

There are three binaries that are part of this distribution.

	tracetool   - the tracing tool utility  
	tracelib.so - the tracing library used by tracetool  
	userapp     - an example application

## BUILDING

tracetool is a standard CMake project. By default, it builds with GNU make.

	$ cmake -DDyninst_DIR=path/to/dyninst/lib/cmake/Dyninst
	$ make

## SETUP
  Before tracetool is run, certain environment variables need to be set.

  For dyninstAPI, the environment variables DYNINSTAPI_RT_LIB and
  LD_LIBRARY_PATH need to be set appropriately.  DYNINSTAPI_RT_LIB needs to
  be set to the path of the dyninst runtime library file
  (libdyninstAPI_RT.so.1) and the environment variable LD_LIBRARY_PATH needs
  to be updated so that it contains the directory containing the dyninst
  library (libdyninstAPI.so).

  For the tracetool utility, the environment variable TRACETOOL_LIB needs to
  be set to the path of the tracetool tracing library.

  Here's example of cshell commands which I use to do the above.
  setenv DYNINSTAPI_RT_LIB path/to/dyninst/lib/libdyninstAPI_RT.so.1
  setenv LD_LIBRARY_PATH path/to/dyninst/lib:$LD_LIBRARY_PATH
  setenv TRACETOOL_LIB path/to/tracetool/tracelib.so


## RUNNING
  As the options below show it can either create or attach to the process to
  be traced.

	$ tracetool
	Usage: tracetool [-p<pid>] program [prog-arguments]
	  -p: specify process id of program for attaching
	  
	  if no pid is passed, then the process is started directly

  An example of an invocation of tracetool on the application wget.

  for attaching (`2567` is the pid of the `wget` process started in another terminal):

	$ tracetool -p2567 $A/sequential/wget/bin/wget

  for creating:

	$ tracetool wget -Q500m -r http://www.example.com/ -o mylog | & tee log1

## CREDITS
Written by Brandon Schendel
