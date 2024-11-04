# A Makefile to build my 'mysync' project.

PROJECT = mysync
HEADERS = $(PROJECT).h
OBJ = mysync.o globals.o directories.o files.o glob2regex.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT): $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o: %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)