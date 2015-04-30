#!/bin/sh
MAKE_FLAGS=DEBUG=y
cd cpp/src;make $MAKE_FLAGS;cd -;


cd cpp/src;make clean;cd -;

rm -rf build;
mkdir -p build/entry_thrift;
mkdir -p build/lib;
mv include/* build/entry_thrift; rm -rf include;
mv lib/* build/lib/ -f; rm -rf lib;

exit
