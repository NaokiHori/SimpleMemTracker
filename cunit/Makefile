PRJCT_ROOT := ..
CC 		     := cc
CFLAGS     := -O0 -g -std=c99 -Wall -Wextra
# directly include src of the project
# to access to the static functions
INCLUDES   := -Iinclude \
							-I$(PRJCT_ROOT)/include \
							-I$(PRJCT_ROOT)/src
LIBS			 := -L/usr/local/lib \
							-lcunit
SRCS1      := src/common.c src/get_nitems.c
TARGET1    := test_get_nitems.out

all: get_nitems

get_nitems:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS1) -o $(TARGET1) $(LIBS)

clean:
	$(RM) *.out *.xml

