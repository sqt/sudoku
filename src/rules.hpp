#pragma once

#include <cstdint>

enum RuleApplied : uint8_t
{
	None = 0,
	RemovingPeers,
	SingleChoice,
	ForcedGroup,
	Intersection,
	SuccessfulGuess,
	FailedGuess,
	RuleAppliedMax
};

extern const char* const RuleStrings[];
