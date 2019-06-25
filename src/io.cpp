#include <iostream>
#include <cmath>
#include <sstream>

#include "io.hpp"

PuzzleState readPuzzleFromFile(std::ifstream& ifs)
{
	PuzzleState result;

	EntryState unknown_entry;
	for (uint8_t i = 1; i < 10; ++i)
		unknown_entry.insert(i);

	for (std::string line; std::getline(ifs, line); )
	{
		if (line.empty() == false)
		{
			RowState row;
			std::stringstream ss(line);
			while (ss.good())
			{
				uint16_t value;
				ss >> value;
				if (value == 0)
				{
					row.push_back(unknown_entry);
				}
				else
				{
					EntryState entry;
					entry.insert(std::min(static_cast<uint8_t>(value), static_cast<uint8_t>(9u)));
					row.push_back(entry);
				}
			}
			result.push_back(row);
		}
	}
	return result;	
}

PuzzleState readPuzzleFromJSON(const std::string& json)
{
	PuzzleState result;

	std::vector<size_t> values;

	size_t sloc = json.find_first_of("[") + 1;
	while (sloc != std::string::npos && sloc < json.size())
	{
		size_t eloc = json.find_first_of(",]", sloc);
		size_t value = std::strtoull(json.substr(sloc, eloc - sloc).data(), NULL, 10);
		values.push_back(value);
		sloc = eloc + 1;
	}	

	size_t root = static_cast<size_t>(std::sqrt(static_cast<double>(values.size())));
	if (root * root != values.size())
	{
		return result;
	}

	EntryState unknown_entry;
	for (size_t i = 1; i < root + 1; ++i)
		unknown_entry.insert(i);

	for (size_t i = 0; i < root; ++i)
	{
		RowState row;
		for (size_t j = 0; j < root; ++j)
		{
			size_t value = values[i * root + j];
			if (value != 0)
			{
				EntryState entry;
				entry.insert(value);
				row.push_back(entry);
			}
			else
			{
				row.push_back(unknown_entry);
			}
		}
		result.push_back(row);
	}

	return result;
}

void printRow(const RowState& row, std::ostream& os)
{
	for (auto j = row.cbegin(); j < row.cend(); ++j)
	{
		if (j->size() == 0) // Zero Options Remaining (Should Not Happen)
		{
			os << "- " << std::flush;
		}
		else if (j->size() == 1)
		{
			os << static_cast<uint16_t>(*(j->cbegin())) << " " << std::flush;
		}
		else
		{
			os << "  " << std::flush;
		}
	}
} 

void printJSONRow(const RowState& row, std::ostream& os)
{
	for (auto j = row.cbegin(); j < row.cend(); ++j)
	{
		if (j != row.cbegin())
		{
			os << ',';
		}

		if (j->size() == 0) // Zero Options Remaining (Should Not Happen)
		{
			os << '0';
		}
		else if (j->size() == 1)
		{
			os << static_cast<uint16_t>(*(j->cbegin()));
		}
		else
		{
			os << ' ';
		}
	}
} 

void printSolution(const PuzzleState& state, std::ostream& os)
{
	for (auto i = state.cbegin(); i < state.cend(); ++i)
	{
		printRow(*i, os);
		os << std::endl;
	}
}

void printJSONSolution(const PuzzleState& state, std::ostream& os)
{
	os << '[';
	for (auto i = state.cbegin(); i < state.cend(); ++i)
	{
		if (i != state.cbegin())
		{
			os << ',';
		}

		printJSONRow(*i, os);
	}
	os << ']' << std::flush;
}


