// Advent of Code 2021
// Day 1

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

static int constexpr DAY = 1;

static int constexpr WINDOW_SIZE = 3;

int64_t computeWindow(std::vector<int64_t>::const_iterator p)
{
	int64_t window = 0;
	for (int i = 0; i < WINDOW_SIZE; ++i)
		window += *(p - i);
	return window;
}

int main(int argc, char** argv)
{
	std::string path;
	int part;

	setup::parseCommandLine(argc, argv, DAY, &path, &part);
	setup::printBanner(DAY, part);

	// Read the file
	auto depths = load::integers(path);

	if (part == 1)
	{
		// Count the number of increasing depths
		int nIncreasingDepths = 0;
		int64_t previousDepth = depths[0];
		for (auto i = depths.begin() + 1; i != depths.end(); ++i)
		{
			int64_t depth = *i;
			if (depth > previousDepth)
				++nIncreasingDepths;
			previousDepth = depth;
		}
		std::cout << "Answer: " << nIncreasingDepths << std::endl;
	}
	else
	{
		// Count the number of increasing windows
		int nIncreasingWindows = 0;
		int64_t previousWindow = computeWindow(depths.begin() + WINDOW_SIZE - 1);
		for (auto i = depths.begin() + WINDOW_SIZE; i != depths.end(); ++i)
		{
			int64_t window = computeWindow(i);
			if (window > previousWindow)
				++nIncreasingWindows;
			previousWindow = window;
		}
		std::cout << "Answer: " << nIncreasingWindows << std::endl;
	}
}
