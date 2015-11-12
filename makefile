################################################################################
# makefile for 'colorcode' #####################################################
################################################################################

COMPILE=g++ -Wall -pedantic-errors -Werror -Wextra -Wshadow -Wfatal-errors \
			-Wno-unused-variable --std=c++11

colorcode.o: colorcode-xterm.cpp colorcode.h
	$(COMPILE) -c -o colorcode.o colorcode-xterm.cpp

all: colorcode.o
example1: colorcode.o example1.cpp
	$(COMPILE) -oexample1 example1.cpp colorcode.o
