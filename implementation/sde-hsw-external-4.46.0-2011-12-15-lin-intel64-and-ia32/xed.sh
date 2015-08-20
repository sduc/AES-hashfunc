#!/bin/bash
# Add the c++ runtime from the kit if xed the normal runtime does not work.

# get the directory of this script
origin=$0
# If it does not begin with /, then it is relative to the current working directory
if [ "${origin:0:1}" != "/" ]; then
    origin=$PWD/$origin
fi
origin=`dirname "$origin"`

cpp_runtime_libs="$origin/intel64/pin_lib:$origin/ia32/pin_lib"
export LD_LIBRARY_PATH=$cpp_runtime_libs:$LD_LIBRARY_PATH
exec "$origin"/xed "${@}"
