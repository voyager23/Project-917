# Makefile for Project#917 folder


RUN = a_file
CFLAGS = --std=c++20 
DEBUG = -g 

a_file : a_file.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	
phony: run gdb

run :
	../bin/$(RUN)
	
gdb:
	gdb ../bin/$(RUN)
