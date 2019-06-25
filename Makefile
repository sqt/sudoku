PROG_CGI = solver.cgi
PROG_CMD = solver

CFLAGS = -std=c++14 -Wall -pedantic -O2 -DNDEBUG -g0
LDFLAGS =
LIBS =
CC = g++

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))

MAIN_SRC = $(wildcard src/*main.cpp)

COMMON_SRC = $(filter-out $(MAIN_SRC), $(SRC))
COMMON_OBJ = $(patsubst src/%.cpp, obj/%.o, $(COMMON_SRC))

all: $(PROG_CGI) $(PROG_CMD)

$(PROG_CGI): $(COMMON_OBJ) obj/cgi_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(COMMON_OBJ) obj/cgi_main.o -o $(PROG_CGI)

$(PROG_CMD): $(COMMON_OBJ) obj/cmd_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(COMMON_OBJ) obj/cmd_main.o -o $(PROG_CMD)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean realclean
clean:
	rm -rf obj/*.o
realclean: clean
	rm $(PROG_CMD) $(PROG_CGI)

