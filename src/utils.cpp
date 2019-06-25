#include <algorithm>
#include <cassert>
#include <cstdint>

#include "utils.hpp"

void buildUnionState(const EntryHouse& house, EntryState& out)
{
	for (auto it = house.begin(); it != house.end(); ++it)
	{
		for (auto eit = (*it)->begin(); eit != (*it)->end(); ++eit)
		{
			out.insert(*eit);
		}
	}
}

bool removeValueFromHouse(const EntryHouse& house, uint8_t value)
{
	size_t numberRemoved = 0;
	for (auto it = house.begin(); it != house.end(); ++it)
	{
		numberRemoved += (*it)->erase(value);
	}
	return numberRemoved != 0;
}

void removeIntersection(const EntryHouse& house, EntryHouse& out)
{
	for (EntryHouse::const_iterator it = house.cbegin(); it != house.cend(); ++it)
	{
		EntryState* ptr = *it;
		EntryHouse::iterator fit = std::find(out.begin(), out.end(), ptr);
		if (fit != out.end())
		{
#ifndef NDEBUG
			size_t old_size = out.size();
#endif
			out.erase(fit);
			assert(out.size() == old_size - 1);
		}
	}
}

