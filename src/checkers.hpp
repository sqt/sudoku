#pragma once

#include "types.hpp"
#include "rules.hpp"

RuleApplied checkRow(PuzzleState& state, size_t index);
RuleApplied checkColumn(PuzzleState& state, size_t index);
RuleApplied checkBlock(PuzzleState& state, size_t index);

RuleApplied checkHouse(EntryHouse& house);
RuleApplied checkIntersections(PuzzleState& state);

bool rowSolved(const RowState& row);
bool puzzleSolved(const PuzzleState& state);

