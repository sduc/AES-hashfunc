
Documentation:

  http://www.intel.com/software/sde

Support is via Intel Software Network Forums:

  http://software.intel.com/en-us/forums/intel-avx-and-cpu-instructions/

==============================================================

FAQ:

1) When using the "xed" command line tool, if you get this runtime error:

    xed: /usr/lib/libstdc++.so.6: version `GLIBCXX_3.4.9' not found 
        (required by xed)

  then you need to set your LD_LIBRARY_PATH environment variable.
  For csh/tcsh:
      setenv LD_LIBRARY_PATH path-to-sde-kit/ia32/pin_lib

  for bash/sh shells:
      export LD_LIBRARY_PATH=path-to-sde-kit/ia32/pin_lib

  where path-to-sde-kit is where you put your kit.

