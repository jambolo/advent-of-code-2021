// Advent of Code 2021
// Day 17

#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 17;

struct Target
{
    int x0, y0, x1, y1;
};
static void to_json(json& j, Target const& t)
{
    j = json::array({ json::array({ t.x0, t.y0 }), json::array({ t.x1, t.y1 }) });
}

static Target extractTarget(std::string const & line);
static void step(int& x, int& y, int& vX, int& vY);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    std::string input = load::string(inputPath);

    // Get the location and size of the target

    Target target = extractTarget(input);

    if (part == 1)
    {
        // The minimum x velocity is the lowest vx0 such that vx slows to 0 when x >= target.x0
        int minVx0 = static_cast<int>(std::ceil((std::sqrt(1.0 + 8.0 * target.x0) - 1.0) / 2.0));

        // The maximum height can be reached when vx slows to 0 before x > target.x1
        int maxVx0 = static_cast<int>(std::floor((std::sqrt(1.0 + 8.0 * target.x1) - 1.0) / 2.0));

        // When the probe returns to y = 0, v = -vy0, so avoid overshooting on the next step
        int maxVy0 = abs(target.y0);

        // Has to be at least 1 to get a positive height
        int minVy0 = 1;

        int maxY = 0;

        for (int vX0 = minVx0; vX0 <= maxVx0; ++vX0)
        {
            for (int vY0 = minVy0; vY0 <= maxVy0; ++vY0)
            {
                int vX = vX0;
                int vY = vY0;
                int x = 0;
                int y = 0;
                int peak = (vY0 * (vY0 + 1)) / 2;
                while (true)
                {
                    if (x >= target.x0 && x <= target.x1 && y >= target.y0 && y <= target.y1)
                    {
                        // Hit
                        maxY = std::max(maxY, peak);
                        break;
                    }
                    else if (y < target.y0 || x > target.x1)
                    {
                        // Miss
                        break;
                    }
                    step(x, y, vX, vY);
                }
            }
        }

        std::cout << "Answer: " << maxY << std::endl;
    }
    else
    {
        // The minimum x velocity is the lowest vx0 such that vx slows to 0 when x >= target.x0
        int minVx0 = static_cast<int>(std::ceil((std::sqrt(1.0 + 8.0 * target.x0) - 1.0) / 2.0));

        // The maximum x velocity must avoid overshooting the target on the first step
        int maxVx0 = target.x1;

        // When the probe returns to y = 0, v = -vy0, so avoid overshooting on the next step
        int maxVy0 = abs(target.y0);

        // The minimum y velocity must avoid overshooting the target on the first step
        int minVy0 = target.y0; // note this is negative

        int maxY = 0;
        int count = 0;
        for (int vX0 = minVx0; vX0 <= maxVx0; ++vX0)
        {
            for (int vY0 = minVy0; vY0 <= maxVy0; ++vY0)
            {
                int vX = vX0;
                int vY = vY0;
                int x = 0;
                int y = 0;
                while (true)
                {
                    if (x >= target.x0 && x <= target.x1 && y >= target.y0 && y <= target.y1)
                    {
                        // Hit
                        ++count;
                        break;
                    }
                    else if (y < target.y0 || x > target.x1)
                    {
                        // Miss
                        break;
                    }
                    step(x, y, vX, vY);
                }
            }
        }

        std::cout << "Answer: " << count << std::endl;
    }


    return 0;
}

static Target extractTarget(std::string const& s)
{
    // Trim the line
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    std::string trimmed = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    std::regex pattern("target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-?[0-9]+)..(-?[0-9]+)");
    std::smatch match;
    bool found = std::regex_match(trimmed, match, pattern);
    if (!found)
        throw std::runtime_error("regex failed");

    return
    {
        std::stoi(match[1].str()),
        std::stoi(match[3].str()),
        std::stoi(match[2].str()),
        std::stoi(match[4].str())
    };
}

static void step(int& x, int& y, int& vX, int& vY)
{
    x += vX;
    y += vY;
    if (vX > 0)
        --vX;
    --vY;
}