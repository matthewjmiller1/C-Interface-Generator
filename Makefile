#
# Makefile: Make an achive
# Copyright (C) 2011  Matt Miller
#
# Based on example from:
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

DIR=c_intf_gen
NAME=$(DIR)

EXAMPLE=abs_factory

.PHONY: tar

tar:
	tar -czvf $(NAME).tar.gz ../$(NAME) --exclude *.swp --exclude *.o \
        --exclude test_$(EXAMPLE) --exclude $(EXAMPLE).tar.gz
