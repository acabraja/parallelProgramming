#!/bin/bash
echo 'kompajliram forthr_c.c ...'
icc -pthread -c interface.c
echo 'kompajliram forthr.f90 ...'
ifort -standard-semantics -c interface.f90 -o interfacef
echo 'kompajliram mymod.f90 ...'
ifort -standard-semantics -c mymodul.f90
echo 'kompajliram demo.f90 i linkam ...'
ifort -standard-semantics fzad1.f90 interfacef.o mymod.o interface.o -o fzad1
echo 'pokreni ./demo_f'
