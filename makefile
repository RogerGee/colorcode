################################################################################
# makefile for 'colorcode' #####################################################
#  builds 'colorcode' library using 'xterm' platform ###########################
################################################################################

COMPILE=g++ -Wall -pedantic-errors -Werror -Wextra -Wshadow -Wfatal-errors \
			-Wno-unused-variable --std=c++11
LIBRARY=/usr/local/lib/libcolorcode.a
HEADER=/usr/local/include/colorcode.h
PLATFORM=xterm

colorcode.o: colorcode-$(PLATFORM).cpp colorcode.h
	$(COMPILE) -c -o colorcode.o colorcode-$(PLATFORM).cpp

all: colorcode.o
install: $(LIBRARY) $(HEADER)
example1: colorcode.o example1.cpp
	$(COMPILE) -s -oexample1 example1.cpp colorcode.o
example2: colorcode.o example2.cpp
	$(COMPILE) -s -oexample2 example2.cpp colorcode.o
example3: colorcode.o example3.cpp
	$(COMPILE) -s -oexample3 example3.cpp colorcode.o
barf: colorcode.o barf.cpp
	$(COMPILE) -s -obarf barf.cpp colorcode.o

$(LIBRARY): colorcode.o
	@ar crv $(LIBRARY) colorcode.o
$(HEADER): colorcode.h
	@cp --verbose colorcode.h $(HEADER)
