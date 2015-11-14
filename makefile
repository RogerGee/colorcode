################################################################################
# makefile for 'colorcode' #####################################################
################################################################################

COMPILE=g++ -Wall -pedantic-errors -Werror -Wextra -Wshadow -Wfatal-errors \
			-Wno-unused-variable --std=c++11
LIBRARY=/usr/local/lib/libcolorcode.a
HEADER=/usr/local/include/colorcode.h

colorcode.o: colorcode-xterm.cpp colorcode.h
	$(COMPILE) -c -o colorcode.o colorcode-xterm.cpp

all: colorcode.o
install: $(LIBRARY) $(HEADER)
example1: colorcode.o example1.cpp
	$(COMPILE) -oexample1 example1.cpp colorcode.o
example2: colorcode.o example2.cpp
	$(COMPILE) -oexample2 example2.cpp colorcode.o

$(LIBRARY): colorcode.o
	@ar crv $(LIBRARY) colorcode.o
$(HEADER): colorcode.h
	@cp --verbose colorcode.h $(HEADER)
