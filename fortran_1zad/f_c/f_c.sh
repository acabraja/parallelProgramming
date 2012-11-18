#!/bin/bash
echo 'kompajliram forthr_c.c ...'
icc -pthread -c forthr_c.c
echo 'kompajliram forthr.f90 ...'
ifort -standard-semantics -c forthr.f90
echo 'kompajliram mymod.f90 ...'
ifort -standard-semantics -c mymod.f90
echo 'kompajliram demo.f90 i linkam ...'
ifort -standard-semantics demo.f90 forthr.o mymod.o forthr_c.o -o demo_f
echo 'kompajliram demo.c ...'
icc -pthread -c demo.c
echo 'linkam demo_c ...'
ifort -standard-semantics -nofor-main demo.o forthr_c.o mymod.o -o demo_c
echo 'kompajliram demo.cpp ...'
icpc -pthread -c demo.cpp
echo 'linkam demo_cpp ...'
ifort -standard-semantics -nofor-main -cxxlib demo.o forthr_c.o mymod.o -o demo_cpp
echo 'pokreni ./demo_f'
echo 'pokreni ./demo_c'
echo 'pokreni ./demo_cpp'
