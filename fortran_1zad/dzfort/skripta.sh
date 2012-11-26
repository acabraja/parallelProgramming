#!/bin/bash
echo 'kompajliram interface.c ...'
icc -pthread -c interface.c
echo 'kompajliram finterface.f90 ...'
ifort -mkl=sequential -standard-semantics -c finterface.f90 
echo 'kompajliram mymodul.f90 ...'
ifort -mkl=sequential -standard-semantics -c mymodul.f90
echo 'kompajliram fzad1.f90 i linkam ...'
ifort -mkl=sequential -standard-semantics fzad1.f90 finterface.o mymodul.o interface.o -o fzad1
echo 'pokreni ./fzad1'
