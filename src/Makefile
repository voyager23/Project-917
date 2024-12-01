# Makefile for Project#917 folder


CFLAGS = --std=c++20 
DEBUG = -g

no_matrix : no_matrix.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin

a_min_path : a_min_path.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin
	
priority_q : priority_q.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin

class_file : class_file.cpp
	g++ $(CFLAGS) $(DEBUG) $@.cpp -o ../bin/$@
	@ln -fs ../bin/$@ ../bin/bin_link
	@symlinks -v ../bin

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
