# Makefile for Project#917 folder


CFLAGS = --std=c++20 
DEBUG = -g

amap : amap.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin
	
phony: run gdb

run :
	../bin/bin_link
