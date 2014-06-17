PROJECT = lists

CFLAGS = -std=c11 -O0 -Wall -Wextra -Winline -pedantic
CC = gcc

ifneq ($(DEBUG), 0)
	CFLAGS += -DDEBUG
endif

SOURCEDIRS = ./
INCLUDEPATH = -I./ -I../../

DEPENDENCIES := $(shell find -name ".depend")
CSOURCES := $(shell find $(SOURCEDIRS) -name "*.c")
COBJECTS = $(CSOURCES:%.c=%.o)
LIBS =

.PHONY: all clean depend memcheck
.SUFFIXES:

all: $(PROJECT)

$(PROJECT): $(COBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(COBJECTS) -o $(PROJECT)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDEPATH) $< -o $@

.depend:
	for src in $(COBJECTS) ; do \
		$(CC) -MM $(CFLAGS) $(INCLUDEPATH) -MT "$$src" "$$(echo $$src | sed s/o$$/c/)" ; \
	done > $@

depend: .depend
	@touch .depend

memcheck:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 ./$(PROJECT)

clean:
	rm -f $(DEPENDENCIES)
	rm -f $(COBJECTS)
	rm -f $(PROJECT)

-include $(DEPENDENCIES)
