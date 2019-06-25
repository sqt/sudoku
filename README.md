# sudoku
A C++ Solver for Variably-Sized Sudoku Puzzles

Attempts to intelligently reduce the number of possible solutions before eventually
guessing & checking answers (only if necessary). Should theoretically be capable of
solving any solvable sudoku.

Currently returns the number of "iterations" required to
solve the puzzle, giving a rough gauge of how "difficult" the puzzle was to solve.

Contains C++ source code for both a command line and cgi variant of the solver,
in addition to two example index.html pages which can be used to invoke the
cgi variant.

To build:
make

To run with a test input:
./solver inputs/easy_puzzle.txt
