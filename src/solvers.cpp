#include "checkers.hpp"
#include "solvers.hpp"

RuleApplied attemptSolution(PuzzleState& state)
{
	RuleApplied changed = None;
	const size_t size = state.size();

	for (size_t i = 0; i < size && changed == None; ++i)
	{
		changed = checkRow(state, i);
		//if (changed != None)
		//	std::cout << "Applied rule to row " << i << " " << RuleStrings[changed] << std::endl;
	}

	for (size_t i = 0; i < size && changed == None; ++i)
	{
		changed = checkColumn(state, i);
		//if (changed != None)
		//	std::cout << "Applied rule to column " << i << " " << RuleStrings[changed] << std::endl;
	}

	for (size_t i = 0; i < size && changed == None; ++i)
	{
		changed = checkBlock(state, i);
		//if (changed != None)
		//	std::cout << "Applied rule to block " << i << " " << RuleStrings[changed] << std::endl;
	}

	if (changed == None)
	{
		changed = checkIntersections(state);
	}

	return changed;
}

Guess makeGuess(PuzzleState& puzzle)
{
	// Find the entry with fewest choices (hopefully leads to quicker solution?)

	size_t min_size = puzzle.size(); // max number of choices will be height / width of puzzle
	size_t min_column = min_size + 1;
	size_t min_row = min_size + 1;

	size_t row = 0;
	for (auto itr = puzzle.begin(); itr != puzzle.end(); ++itr)
	{
		size_t column = 0;
		for (auto ite = itr->begin(); ite != itr->end(); ++ite)
		{
			if (ite->size() > 1 && ite->size() < min_size)
			{
				min_column = column;
				min_row = row;
				min_size = ite->size();
			}
			++column;
		}
		++row;
	}

	if (min_column < puzzle.size() && min_row < puzzle.size())
	{
		EntryState& entry = puzzle[min_row][min_column];
		size_t value = *(entry.begin());
		entry.clear();
		entry.insert(value);
		return Guess(min_row, min_column, value);
	}
	else
	{
		return Guess();
	}
}

void removeGuess(PuzzleState& puzzle, const Guess& guess)
{
	puzzle[guess.row][guess.column].erase(guess.value);
}

bool solvePuzzle(PuzzleState& puzzle, RuleCounts& counts)
{
	RuleApplied rule = None;
	try
	{
		rule = attemptSolution(puzzle);
		while (rule != None)
		{
			counts[rule]++;
			rule = attemptSolution(puzzle);
		}
	}
	catch (const std::exception&)
	{
		return false;
	}

	if (rule == None && puzzleSolved(puzzle) == false)
	{
		auto puzzleCopy = puzzle;
		auto countsCopy = counts;

		counts[SuccessfulGuess]++;
		auto guess = makeGuess(puzzle);

		if (guess.IsValid() == false)
		{
			return false;
		}

		if (solvePuzzle(puzzle, counts) == false)
		{
			puzzle = puzzleCopy;
			counts = countsCopy;

			counts[FailedGuess]++;
			removeGuess(puzzle, guess);

			return solvePuzzle(puzzle, counts);
		}
	}

	return true;
}

