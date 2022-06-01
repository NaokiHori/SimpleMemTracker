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
SRCS2      := src/common.c src/add.c
SRCS3      := src/common.c src/remove.c
TARGET1    := test_get_nitems.out
TARGET2    := test_add.out
TARGET3    := test_remove.out

all: get_nitems add remove

get_nitems:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS1) -o $(TARGET1) $(LIBS)

add:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS2) -o $(TARGET2) $(LIBS)

remove:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS3) -o $(TARGET3) $(LIBS)

clean:
	$(RM) *.out *.xml

