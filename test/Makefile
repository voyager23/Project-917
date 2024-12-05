# Makefile for Project#917 test folder

CFLAGS = --std=c++20 
DEBUG = -g

mappairs : mappairs.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin

bmap : bmap.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin

amap : amap.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin
	
phony: run gdb

run :
	../bin/bin_link

gdb :
	gdb ../bin/bin_link
