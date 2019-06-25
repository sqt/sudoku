#pragma once

#include "types.hpp"

void buildRow(PuzzleState& state, size_t index, EntryHouse& house);
void buildColumn(PuzzleState& state, size_t index, EntryHouse& house);
void buildBlock(PuzzleState& state, size_t index, EntryHouse& house);

void buildRowBlockIntersection(PuzzleState& state, size_t row_index, size_t block_index, EntryHouse& house);
void buildColumnBlockIntersection(PuzzleState& state, size_t column_index, size_t block_index, EntryHouse& house);

