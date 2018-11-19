#!/bin/bash

BUILD_DIR=""
EXT=""

if [ "$2" == "linux" ]
then
BUILD_DIR="linux/"
EXT=".so"
elif [ "$2" == "nt" ]
then
BUILD_DIR="nt/"
EXT=".dll"
else
exit -1
fi

cp ../../cexception/Debug/libcexception$EXT ../../$1/build/$BUILD_DIR
cp ../../unity/Debug/libunity$EXT ../../$1/build/$BUILD_DIR

cd ../../$1/build/$BUILD_DIR
sed -i "s&^EXTRA_CC_FLAGS = .*&EXTRA_CC_FLAGS = -I../../../cexception/ -I../../../unity/&g" Makefile
sed -i "s/^EXTRA_LIBS = .*/EXTRA_LIBS = libcexception$EXT libunity$EXT/g" Makefile
