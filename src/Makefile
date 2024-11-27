# Makefile for Project#917 folder


CFLAGS = --std=c++20 
DEBUG = -g 

b_file : b_file.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin
		
a_file : a_file.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin	
	
phony: run gdb

run :
	../bin/bin_link
