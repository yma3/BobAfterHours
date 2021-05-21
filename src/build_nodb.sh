#! /bin/sh
CC=g++
CFLAGS="-I /home/ben/Documents/json/include"
FLAGS="main.cpp Board.cpp Minion.cpp StatTracker.cpp"
OFILE="main_nodb.out"

$CC $FLAGS $CFLAGS -o $OFILE

