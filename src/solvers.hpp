#pragma once

#include "rules.hpp"
#include "types.hpp"

RuleApplied attemptSolution(PuzzleState& state);
bool solvePuzzle(PuzzleState& puzzle, RuleCounts& counts);

Guess makeGuess(PuzzleState& puzzle);
void removeGuess(PuzzleState& puzzle, const Guess& guess);

