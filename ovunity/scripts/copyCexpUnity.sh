#!/bin/bash

cp ../../cexception/Debug/libcexception.so ../../$1/build/linux/
cp ../../unity/Debug/libunity.so ../../$1/build/linux/

cd ../../$1/build/linux
sed -i "s&^EXTRA_CC_FLAGS = .*&EXTRA_CC_FLAGS = -I../../../cexception/ -I../../../unity/&g" Makefile
sed -i "s/^EXTRA_LIBS = .*/EXTRA_LIBS = libcexception.so libunity.so/g" Makefile