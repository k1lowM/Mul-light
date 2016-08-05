#!/bin/sh

make -C Include/Mul_light/ remake

make -C Application/Sample1/ remake
make -C Application/Sample2/ remake
make -C Application/Sample3/ remake

make