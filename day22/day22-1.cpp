// Advent of Code 2021
// Day 22, part 1

#include "Cube.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 22;

//#define TEST    1


struct Step
{
    bool on;
    Cube region;
};

void to_json(json& j, Step const& s);

std::array<std::array<std::array<bool, 101>, 101>, 101> reactor;

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    part = 1;    // Override the command line parameter
    setup::printBanner(DAY, part);

    // Read the input
    auto lines = load::lines(inputPath);

    // Parse the lines
    std::vector<Step> steps;
    Cube bounds(-50, 50, -50, 50, -50, 50);

    std::regex regex("(on|off) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+)");
    for (auto const& line : lines)
    {
        std::smatch match;
        if (std::regex_match(line, match, regex))
        {
            Step step{
                match[1].str() == "on",
                Cube(
                std::stoi(match[2].str()),
                std::stoi(match[3].str()),
                std::stoi(match[4].str()),
                std::stoi(match[5].str()), 
                std::stoi(match[6].str()),
                std::stoi(match[7].str())
                )
            };
            if (bounds.intersects(step.region))
                steps.push_back(step);
        }
    }

    // Initialize the reactor
    for (auto const& step : steps)
    {
        for (int x = step.region.minX_; x <= step.region.maxX_; ++x)
        {
            for (int y = step.region.minY_; y <= step.region.maxY_; ++y)
            {
                for (int z = step.region.minZ_; z <= step.region.maxZ_; ++z)
                {
                    reactor[x+50][y+50][z+50] = step.on;
                }
            }
        }
    }

    // Count the cubes that are on
    int count = 0;
    for (auto const& x : reactor)
    {
        for (auto const& y : x)
        {
            for (auto z : y)
            {
                if (z)
                    ++count;
            }
        }
    }

    std::cout << "Answer: " << count << std::endl;
    return 0;
}

void to_json(json& j, Step const& s)
{
    j["on"] = s.on;
    j["region"] = s.region;
}
