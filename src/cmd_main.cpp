#include <algorithm>
#include <iostream>
#include <sstream>

#include "io.hpp"
#include "solvers.hpp"
#include "types.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: unexpected number of input arguments." << std::endl;
		return -1;
	}

	bool fromFile = true;
	PuzzleState puzzle;

	if (argv[1][0] != '[')
	{
		std::ifstream ifs(argv[1]);
		if (!ifs.good())
		{
			std::cerr << "Error: unable to open input file." << std::endl;
			return -2;
		}
		
		puzzle = readPuzzleFromFile(ifs);
	}
	/*else
	{
		fromFile = false;
		puzzle = readPuzzleFromJSON(std::string(argv[1]));
	}
	*/

	if (puzzle.empty())
		return -3;

	try
	{
		RuleCounts counts(RuleAppliedMax, 0);
		auto result = solvePuzzle(puzzle, counts);

		size_t total_count = 0;
		std::for_each(counts.cbegin(), counts.cend(),[&](size_t val)
			{
				total_count += val;
			});

		if (fromFile)
		{
			if (result)
			{
				std::cout << "Solution found in " << total_count << " iterations!" << std::endl;
			}
			else
			{
				std::cout << "Solution not found in " << total_count << " iterations. :(" << std::endl;
			}

			printSolution(puzzle, std::cout);
		}
		else
		{
			std::ostringstream oss;
			oss << "Content-type: text/html" << std::endl;
			oss << std::endl;
			printJSONSolution(puzzle, oss);
			std::cout << oss.str() << std::endl;
		}
	}		
	catch (const std::exception& e)
	{
		std::cerr << "Caught std::exception during read / solve. Exiting. Message: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Caught exception during read / solve. Exiting." << std::endl;
	}

	return 0;
}


