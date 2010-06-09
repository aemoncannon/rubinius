#!/bin/bash

echo "Method JIT:"
~/src/misc/rubinius_interp/bin/rbx $1 $2

echo "Interpreted:"
~/src/misc/rubinius_no_jit/bin/rbx $1 $2

echo "MRI:"
ruby $1 $2

echo "Trace JIT:"
~/src/misc/rubinius/bin/rbx $1 $2 trace