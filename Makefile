CC       := cc
CFLAGS   := -O3 -std=c99 -Wall -Wextra#-DLOGGING_SMT
INCLUDES := -Iinclude
SRCSDIR  := src
SRCS     := $(foreach dir, $(shell find $(SRCSDIR) -type d), $(wildcard $(dir)/example*.c))
TARGETS  := $(subst $(SRCSDIR)/,,$(SRCS:.c=.out))

all: $(TARGETS)

%.out: $(SRCSDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) $(SRCSDIR)/simple_mem_tracker.c $< -o $@

clean:
		$(RM) $(TARGETS) *.xml

.PHONY : all clean
