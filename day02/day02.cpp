// Advent of Code 2021
// Day 2

#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int const DAY = 2;

struct Move
{
	std::string direction;
	int distance;
};

void to_json(json & j, Move const & m)
{
	j = json{ {"direction", m.direction}, {"distance", m.distance} };
}

void from_json(json const & j, Move & m)
{
	j.at("direction").get_to(m.direction);
	j.at("distance").get_to(m.distance);
}

int main(int argc, char ** argv)
{
	std::string path;
	int part;

	setup::parseCommandLine(argc, argv, DAY, &path, &part);
	setup::printBanner(DAY, part);

    auto lines = load::lines(path);

	std::vector <Move> moves;

	for (auto const& line : lines)
	{
		auto space = line.find(' ');
		if (space == std::string::npos)
			throw std::runtime_error("Invalid move: " + line);
		std::string direction = line.substr(0, space);
		int distance = std::stoi(line.substr(space + 1));
		moves.push_back({ direction, distance });
    }

//	json j = moves;
//	std::cout << j << std::endl;

	if (part == 1)
	{
		int distance = 0;
		int depth = 0;

		for (auto const& m : moves)
		{
			if (m.direction == "forward")
			{
				distance += m.distance;
			}
			else if (m.direction == "up")
			{
				depth -= m.distance;
			}
			else if (m.direction == "down")
			{
				depth += m.distance;
			}
			else
				throw std::runtime_error("Invalid direction: " + m.direction);
		}

		std::cout << "Answer: " << distance * depth << std::endl;

	}
	else
	{
		int distance = 0;
		int aim = 0;
		int aimedDepth = 0;

		for (auto const& m : moves)
		{
			if (m.direction == "forward")
			{
				distance += m.distance;
				aimedDepth += aim * m.distance;
			}
			else if (m.direction == "up")
			{
				aim -= m.distance;
			}
			else if (m.direction == "down")
			{
				aim += m.distance;
			}
			else
				throw std::runtime_error("Invalid direction: " + m.direction);
		}

		std::cout << "Answer: " << distance * aimedDepth << std::endl;
	}

	return 0;
}
