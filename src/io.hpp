#pragma once

#include <fstream>
#include <string>

#include "types.hpp"

PuzzleState readPuzzleFromFile(std::ifstream& ifs);
PuzzleState readPuzzleFromJSON(const std::string& json);

void printRow(const RowState& row, std::ostream& os);
void printSolution(const PuzzleState& state, std::ostream& os);

void printJSONRow(const RowState& row, std::ostream& os);
void printJSONSolution(const PuzzleState& state, std::ostream& os);

