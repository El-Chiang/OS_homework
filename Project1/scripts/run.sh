#!/bin/bash
#=====================================================
# Compile user executable file
#=====================================================
step="Compile user executable file"
echo "----------------------------------------------"
echo "$step"
gcc ../source/usertask.c -o ../build/usertask

#=====================================================
# Compile process.c
#=====================================================
step="Compile process.c"
echo "----------------------------------------------"
echo "$step"
gcc ../source/process.c -o ../build/process.out -lpthread -lm

#=====================================================
# Compile process.c
#=====================================================
step="Execute"
echo "----------------------------------------------"
echo "$step"
echo "----------------------------------------------"
../build/process.out