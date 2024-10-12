PROG_CGI = solver.cgi
PROG_CMD = solver

CFLAGS = -std=c++14 -Wall -pedantic -O2 -DNDEBUG -g0
LDFLAGS =
LIBS =
CC = g++

OBJDIR = obj

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, ${OBJDIR}/%.o, $(SRC))

MAIN_SRC = $(wildcard src/*main.cpp)

COMMON_SRC = $(filter-out $(MAIN_SRC), $(SRC))
COMMON_OBJ = $(patsubst src/%.cpp, ${OBJDIR}/%.o, $(COMMON_SRC))

all: $(PROG_CGI) $(PROG_CMD)

$(PROG_CGI): $(COMMON_OBJ) ${OBJDIR}/cgi_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(COMMON_OBJ) ${OBJDIR}/cgi_main.o -o $(PROG_CGI)

$(PROG_CMD): $(COMMON_OBJ) ${OBJDIR}/cmd_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(COMMON_OBJ) ${OBJDIR}/cmd_main.o -o $(PROG_CMD)

${OBJDIR}/%.o: src/%.cpp ${OBJDIR}
	$(CC) $(CFLAGS) -c $< -o $@

${OBJDIR}:
	mkdir -p ${OBJDIR}

.PHONY: clean realclean
clean:
	rm -rf ${OBJDIR}/*.o
realclean: clean
	rm $(PROG_CMD) $(PROG_CGI)

