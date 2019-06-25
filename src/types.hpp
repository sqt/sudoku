#pragma once

#include <cstddef>
#include <set>
#include <vector>

typedef std::set<size_t> EntryState;
typedef std::vector<EntryState> RowState;
typedef std::vector<RowState> PuzzleState;

typedef std::vector<EntryState*> EntryHouse;

struct Guess
{
	Guess(): row(-1), column(-1), value(-1) {}
	Guess(size_t r, size_t c, size_t v) : row(r), column(c), value(v) {}
	bool IsValid() { return value != static_cast<size_t>(-1); }
	size_t row;
	size_t column;
	size_t value;
};

typedef std::vector<size_t> RuleCounts;

