#include <cmath>

#include "builders.hpp"

void buildRow(PuzzleState& state, size_t index, EntryHouse& house)
{
	for (auto i = state[index].begin(); i < state[index].end(); ++i)
	{
		house.emplace_back(&(*i));
	}
}

void buildColumn(PuzzleState& state, size_t index, EntryHouse& house)
{
	for (auto i = state.begin(); i < state.end(); ++i)
	{
		house.emplace_back(&(*i)[index]);
	}
}

void buildBlock(PuzzleState& state, size_t index, EntryHouse& house)
{
	size_t root = std::sqrt(state.size());

	const size_t sx = (index % root) * root;
	const size_t ex = (index % root) * root + root;
	const size_t sy = (index / root) * root;
	const size_t ey = (index / root) * root + root;

	for (size_t y = sy; y < ey; ++y)
	{
		for (size_t x = sx; x < ex; ++x)
		{
			house.emplace_back(&state[y][x]);
		}
	}

	// This was the hardcoded version for a 9x9 sudoku
	/*
	size_t midx = 4;
	size_t midy = 4;

	if (index < 3)
		midy -= 3;	
	if (index > 5)
		midy += 3;	
	if (index % 3 == 0)
		midx -= 3;	
	if (index % 3 == 2)
		midx += 3;	

	house.emplace_back(&state[midy - 1][midx - 1]);
	house.emplace_back(&state[midy - 1][midx - 0]);
	house.emplace_back(&state[midy - 1][midx + 1]);
	house.emplace_back(&state[midy - 0][midx - 1]);
	house.emplace_back(&state[midy - 0][midx - 0]);
	house.emplace_back(&state[midy - 0][midx + 1]);
	house.emplace_back(&state[midy + 1][midx - 1]);
	house.emplace_back(&state[midy + 1][midx - 0]);
	house.emplace_back(&state[midy + 1][midx + 1]);
	*/
}

void buildRowBlockIntersection(PuzzleState& state, size_t row_index, size_t block_index, EntryHouse& house)
{
	size_t root = std::sqrt(state.size());

	const size_t sx = (block_index % root) * root;
	const size_t ex = (block_index % root) * root + root;
	const size_t sy = (block_index / root) * root;
	const size_t ey = (block_index / root) * root + root;

	if (row_index >= sy && row_index < ey)
	{
		for (size_t x = sx; x < ex; ++x)
		{
			house.emplace_back(&state[row_index][x]);
		}
	}

	// This was the hardcoded version for a 9x9 sudoku
	/*
	size_t midx = 4;
	size_t midy = 4;

	if (block_index < 3)
		midy -= 3;	
	if (block_index > 5)
		midy += 3;	
	if (block_index % 3 == 0)
		midx -= 3;	
	if (block_index % 3 == 2)
		midx += 3;	

	if ((row_index / 3) == (block_index / 3))
	{
		switch (row_index % 3)
		{
			case 0:
			{
				house.emplace_back(&state[midy - 1][midx - 1]);
				house.emplace_back(&state[midy - 1][midx - 0]);
				house.emplace_back(&state[midy - 1][midx + 1]);
				break;
			}
			case 1:
			{
				house.emplace_back(&state[midy - 0][midx - 1]);
				house.emplace_back(&state[midy - 0][midx - 0]);
				house.emplace_back(&state[midy - 0][midx + 1]);
				break;
			}
			case 2:
			{
				house.emplace_back(&state[midy + 1][midx - 1]);
				house.emplace_back(&state[midy + 1][midx - 0]);
				house.emplace_back(&state[midy + 1][midx + 1]);
				break;
			}
		}
	}
	*/
}

void buildColumnBlockIntersection(PuzzleState& state, size_t column_index, size_t block_index, EntryHouse& house)
{
	size_t root = std::sqrt(state.size());

	const size_t sx = (block_index % root) * root;
	const size_t ex = (block_index % root) * root + root;
	const size_t sy = (block_index / root) * root;
	const size_t ey = (block_index / root) * root + root;

	if (column_index >= sx && column_index < ex)
	{
		for (size_t y = sy; y < ey; ++y)
		{
			house.emplace_back(&state[y][column_index]);
		}
	}

	// This was the hardcoded version for a 9x9 sudoku
	/*
	size_t midx = 4;
	size_t midy = 4;

	if (block_index < 3)
		midy -= 3;	
	if (block_index > 5)
		midy += 3;	
	if (block_index % 3 == 0)
		midx -= 3;	
	if (block_index % 3 == 2)
		midx += 3;	

	if ((column_index / 3) == (block_index % 3))
	{
		switch (column_index % 3)
		{
			case 0:
			{
				house.emplace_back(&state[midy - 1][midx - 1]);
				house.emplace_back(&state[midy - 0][midx - 1]);
				house.emplace_back(&state[midy + 1][midx - 1]);
				break;
			}
			case 1:
			{
				house.emplace_back(&state[midy - 1][midx - 0]);
				house.emplace_back(&state[midy - 0][midx - 0]);
				house.emplace_back(&state[midy + 1][midx - 0]);
				break;
			}
			case 2:
			{
				house.emplace_back(&state[midy - 1][midx + 1]);
				house.emplace_back(&state[midy - 0][midx + 1]);
				house.emplace_back(&state[midy + 1][midx + 1]);
				break;
			}
		}
	}
	*/
}

