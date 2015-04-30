#!/bin/sh
MAKE_FLAGS=DEBUG=y
cd cpp/include;make $MAKE_FLAGS;cd -;

cd cpp/include;make clean;cd -;

mkdir -p build
mv include/* build/; rm -rf include
mv lib build/

exit
