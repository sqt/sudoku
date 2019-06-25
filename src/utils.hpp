#pragma once

#include "types.hpp"

void buildUnionState(const EntryHouse& house, EntryState& out);
bool removeValueFromHouse(const EntryHouse& house, uint8_t value);
void removeIntersection(const EntryHouse& house, EntryHouse& out);

