#include <cassert>
#include <map>
#include <stdexcept>

#include "builders.hpp"
#include "checkers.hpp"
#include "utils.hpp"

RuleApplied checkRow(PuzzleState& state, size_t index)
{
	EntryHouse house;
	buildRow(state, index, house);
	return checkHouse(house);
}

RuleApplied checkColumn(PuzzleState& state, size_t index)
{
	EntryHouse house;
	buildColumn(state, index, house);
	return checkHouse(house);
}

RuleApplied checkBlock(PuzzleState& state, size_t index)
{
	EntryHouse house;
	buildBlock(state, index, house);
	return checkHouse(house);
}

RuleApplied checkHouse(EntryHouse& house)
{
	// Anything with only one possibility is a "known" spot, so make a list of those
	EntryState knownSpots;
	for (auto k = house.cbegin(); k < house.cend(); ++k)
	{
		if ((*k)->empty())
			throw std::runtime_error("Nope!");
		
		if ((*k)->size() == 1)
			knownSpots.insert(*((*k)->cbegin()));
	}

	// If we know this whole house let's not waste any more time here
	if (knownSpots.size() == house.size())
		return None;

	RuleApplied result = None;

	// Strike out the obvious (known) conflicts
	if (knownSpots.size() != 0)
	{
		for (auto i = house.begin(); i < house.end(); ++i)
		{
			if ((*i)->size() > 1)
			{
				for (auto j = knownSpots.cbegin(); j != knownSpots.cend(); ++j)
				{
					auto k = (*i)->find(*j);
					if (k != (*i)->end())
					{
						(*i)->erase(k);
						result = RemovingPeers;
					}
				}
			}
		}
	}

	if (result != None)
		return result;

	// Look for entry states where one entry is the only one to have a certain value (and so must be that value)
	for (uint16_t i = 0; i < house.size(); i++)
	{
		size_t count = 0;
		auto last = house.end();
		for (auto j = house.begin(); j < house.end(); ++j)
		{
			auto k = (*j)->find(i);
			if (k != (*j)->end())
			{
				last = j;
				count++;
			}
		}
		if (count == 1 && (*last)->size() > 1)
		{
			(*last)->clear();
			(*last)->insert(i);
			result = SingleChoice;
		}
	}

	if (result != None)
		return result;

	// How often does each possible _combination_ of entry values show up in our house? (entryCounts)
	std::map<EntryState, uint16_t> entryCounts;
	for (auto it = house.begin(); it != house.end(); ++it)
	{
		auto fit = entryCounts.find(*(*it));
		if (fit == entryCounts.end())
		{
			entryCounts[*(*it)] = 1;
		}
		else
		{
			entryCounts[*(*it)]++;
		}
	}

	// When our entry combination counts (see above) are non-trivial (non-singular/known, non-full-house)
	// and when there's a group where the entry size is the same as the combo count size ("1 2" in example below)
	// then those entry values are spoken for in the whole house, so remove those values from other entries
	// The code below does this one value for one entry at a time (slow, but will get them all eventualy)
	// 1 2                 =>   1 2
	// 1 2                 =>   1 2
	// 1 2 3 4 5 6         =>   3 4 5 6 (iteration after 1 & 2 removed)
	// 1 2 3 4 5 6         =>   3 4 5 6
	// 1 2 3 4 5 6         =>   3 4 5 6
	// 1 2 3 4 5 6         =>   3 4 5 6
	// 1 2 3 4 5 6 7 8 9   =>   7 8 9   (iteration after 1 & 2 removed and then 3, 4, 5, 6 removed) (2 iterations later)
	// 1 2 3 4 5 6 7 8 9   =>   7 8 9
	// 1 2 3 4 5 6 7 8 9   =>   7 8 9
	for (auto it = entryCounts.begin(); it != entryCounts.end() && result == None; ++it)
	{
		if (it->first.size() > 1 && it->first.size() < house.size() && it->first.size() == it->second)
		{
			const EntryState& claimedValues = it->first;
			for (auto eit = claimedValues.begin(); eit != claimedValues.end(); ++eit)
			{
				for (auto it = house.begin(); it != house.end(); ++it)
				{
					if (*(*it) != claimedValues)
					{
						if ((*it)->erase(*eit) != 0)
							result = ForcedGroup;
					}
				}
			}
		}		
	}

	return result;
}

RuleApplied checkIntersections(PuzzleState& state)
{
	RuleApplied changed = None;

	const size_t houseMax = state.front().size();
#ifndef NDEBUG
	const size_t intersectionMax = std::sqrt(houseMax);
#endif

	size_t rowBlockIntersectionTotal = 0;
	size_t columnBlockIntersectionTotal = 0;
	for (size_t i = 0; i < houseMax; ++i)
	{
		EntryHouse block;
		buildBlock(state, i, block);

		size_t rowBlockIntersectionCount = 0;
		for (size_t j = 0; j < houseMax; ++j)
		{
			EntryHouse intersection;
			buildRowBlockIntersection(state, j, i, intersection);
			
			if (!intersection.empty())
			{
				assert(intersection.size() == intersectionMax);
				rowBlockIntersectionCount++;

				EntryHouse row;
				buildRow(state, j, row);

				EntryHouse blockWithoutIntersection(block);
				EntryHouse rowWithoutIntersection(row);

				assert(blockWithoutIntersection.size() == houseMax);
				assert(rowWithoutIntersection.size() == houseMax);

				removeIntersection(intersection, blockWithoutIntersection);
				removeIntersection(intersection, rowWithoutIntersection);

				assert(blockWithoutIntersection.size() == houseMax - intersectionMax);
				assert(rowWithoutIntersection.size() == houseMax - intersectionMax);

				EntryState intersectionValuesUnion;
				buildUnionState(intersection, intersectionValuesUnion);
				EntryState blockValuesUnion;
				buildUnionState(blockWithoutIntersection, blockValuesUnion);
				EntryState rowValuesUnion;
				buildUnionState(rowWithoutIntersection, rowValuesUnion);

				for (auto it = intersectionValuesUnion.begin(); it != intersectionValuesUnion.end(); ++it)
				{
					if (blockValuesUnion.find(*it) == blockValuesUnion.end())
					{
						if (removeValueFromHouse(rowWithoutIntersection, *it))
							changed = Intersection;
					}
					else if (rowValuesUnion.find(*it) == rowValuesUnion.end())
					{
						if (removeValueFromHouse(blockWithoutIntersection, *it))
							changed = Intersection;
					}
				}
			}
		}
		assert(rowBlockIntersectionCount == intersectionMax);

		size_t columnBlockIntersectionCount = 0;
		for (size_t j = 0; j < houseMax; ++j)
		{
			EntryHouse intersection;
			buildColumnBlockIntersection(state, j, i, intersection);

			if (!intersection.empty())
			{
				assert(intersection.size() == intersectionMax);
				columnBlockIntersectionCount++;

				EntryHouse column;
				buildColumn(state, j, column);

				EntryHouse blockWithoutIntersection(block);
				EntryHouse columnWithoutIntersection(column);

				assert(blockWithoutIntersection.size() == houseMax);
				assert(columnWithoutIntersection.size() == houseMax);

				removeIntersection(intersection, blockWithoutIntersection);
				removeIntersection(intersection, columnWithoutIntersection);

				assert(blockWithoutIntersection.size() == houseMax - intersectionMax);
				assert(columnWithoutIntersection.size() == houseMax - intersectionMax);

				EntryState intersectionValuesUnion;
				buildUnionState(intersection, intersectionValuesUnion);
				EntryState blockValuesUnion;
				buildUnionState(blockWithoutIntersection, blockValuesUnion);
				EntryState columnValuesUnion;
				buildUnionState(columnWithoutIntersection, columnValuesUnion);

				for (auto it = intersectionValuesUnion.begin(); it != intersectionValuesUnion.end(); ++it)
				{
					if (blockValuesUnion.find(*it) == blockValuesUnion.end())
					{
						if (removeValueFromHouse(columnWithoutIntersection, *it))
							changed = Intersection;
					}
					else if (columnValuesUnion.find(*it) == columnValuesUnion.end())
					{
						if (removeValueFromHouse(blockWithoutIntersection, *it))
							changed = Intersection;
					}
				}
			}
		}
		assert(columnBlockIntersectionCount == intersectionMax);

		rowBlockIntersectionTotal += rowBlockIntersectionCount;
		columnBlockIntersectionTotal += columnBlockIntersectionCount;
	}		
	assert(rowBlockIntersectionTotal == houseMax * intersectionMax);
	assert(columnBlockIntersectionTotal == houseMax * intersectionMax);

	return changed;
}

size_t series_sum(size_t n)
{
	return n == 1 ? 1 : n + series_sum(n - 1);	
}

size_t fact(size_t n)
{
	return n == 1 ? 1 : n * fact(n - 1);	
}

bool rowSolved(const RowState& row)
{
	size_t sum = 0;
	size_t product = 1;
	for (auto j = row.cbegin(); j < row.cend(); ++j)
	{
		if (j->size() != 1)
			return false;
		sum += *(j->cbegin());
		product *= *(j->cbegin());
	}
	return sum == series_sum(row.size()) && product == fact(row.size());
} 

bool puzzleSolved(const PuzzleState& state)
{
	for (auto i = state.cbegin(); i < state.cend(); ++i)
	{
		if (rowSolved(*i) == false)
			return false;
	}
	return true;
}

