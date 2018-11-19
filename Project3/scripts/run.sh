#!/bin/bash
#=====================================================
# Compile ProcessScheduling.c
#=====================================================
step="Compile ProcessScheduling.c"
echo "----------------------------------------------"
echo "$step"
gcc ./source/ProcessScheduling.c -lpthread -o ./build/ProcessScheduling

#=====================================================
# Execute ProcessScheduling
#=====================================================
step="Execute"
echo "----------------------------------------------"
echo "$step"
echo "----------------------------------------------"
./build/ProcessScheduling