#!/bin/bash

echo "Trace JIT:"
~/src/misc/rubinius/bin/rbx $1
echo "Method JIT:"
~/src/misc/rubinius_interp/bin/rbx $1
echo "Interpreted:"
~/src/misc/rubinius_no_jit/bin/rbx $1