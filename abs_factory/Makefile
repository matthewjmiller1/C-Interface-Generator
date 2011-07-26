#
# Makefile: Build and clean the program
# Copyright (C) 2011  Matt Miller
#
# Based on example from:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

#IDIR =../include
CC=gcc
#CFLAGS=-I$(IDIR)
CFLAGS=-Wall -g

ODIR=obj
#LDIR =../lib

#LIBS=-lm
LIBS=

DIR=abs_factory
NAME=$(DIR)

#_DEPS = hellomake.h
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
DEPS = win_factory.h win_button.h osx_factory.h osx_button.h

GEN_DIR=gen
GEN_SUFFIX=_gen
GEN_SCRIPT = ../c_intf_gen.py
GEN_INPUT = abs_factory_def.txt
GEN_SRC = gui_factory$(GEN_SUFFIX).c win_factory$(GEN_SUFFIX).c \
    button$(GEN_SUFFIX).c win_button$(GEN_SUFFIX).c \
    osx_factory$(GEN_SUFFIX).c osx_button$(GEN_SUFFIX).c
GEN_HDR = gui_factory$(GEN_SUFFIX).h gui_factory_friend$(GEN_SUFFIX).h \
    win_factory$(GEN_SUFFIX).h button$(GEN_SUFFIX).h \
    button_friend$(GEN_SUFFIX).h win_button$(GEN_SUFFIX).h \
    osx_factory$(GEN_SUFFIX).h osx_button$(GEN_SUFFIX).h
GEN_DEPS = $(patsubst %,$(GEN_DIR)/%,$(GEN_HDR))
GEN_FILES = $(patsubst %,$(GEN_DIR)/%,$(GEN_SRC) $(GEN_HDR))

_OBJ = gui_factory$(GEN_SUFFIX).o button$(GEN_SUFFIX).o win_factory.o \
    win_button.o osx_factory.o osx_button.o test_$(NAME).o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(GEN_FILES): $(GEN_SCRIPT) $(GEN_INPUT)
	$(GEN_SCRIPT) -s $(GEN_SUFFIX) -o $(GEN_DIR) $(GEN_INPUT)

$(ODIR)/%.o: %.c $(DEPS) $(GEN_FILES)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(GEN_DIR)/%.c $(GEN_DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

test_$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: test_$(NAME)

.PHONY: clean doc

clean:
	rm -f test_$(name) $(ODIR)/*.o *~ core $(GEN_FILES)

doc:
	doxygen
	cd doc/latex; pdflatex refman.tex

