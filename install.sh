#!/bin/sh
MAKE_FLAGS=DEBUG=y
cd cpp/src;make $MAKE_FLAGS;cd -;


cd cpp/src;make clean;cd -;

exit
