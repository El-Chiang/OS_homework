#!/bin/bash
#=====================================================
# Compile communication.c
#=====================================================
step="Compile communication.c"
echo "----------------------------------------------"
echo "$step"
gcc ./source/communication.c -o ./build/communication.out

#=====================================================
# Execute communication
#=====================================================
step="Execute"
echo "----------------------------------------------"
echo "$step"
echo "----------------------------------------------"
./build/communication.out