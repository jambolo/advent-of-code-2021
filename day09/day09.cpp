// Advent of Code 2021
// Day 9

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 9;
static int constexpr WIDTH = 100;
static int constexpr HEIGHT = 100;

using HeightMap = std::array<std::array<int, WIDTH>, HEIGHT>;

static HeightMap buildMap(std::vector<std::string> const & lines);
static bool isLowPoint(HeightMap const& heights, int r, int c);
static int fillBasin(HeightMap & heights, int r, int c);
int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    auto lines = load::lines(inputPath);

    HeightMap heights = buildMap(lines);

    if (part == 1)
    {
        std::vector<std::pair<int, int>> lows;

        // Find the low points
        for (int r = 0; r < HEIGHT; ++r)
        {
            for (int c = 0; c < WIDTH; ++c)
            {
                if (isLowPoint(heights, r, c))
                    lows.emplace_back(std::make_pair(r, c));
            }
        }

        int risk = std::reduce(lows.begin(), lows.end(), 0, [heights](int acc, std::pair<int, int> x)
            {
                return acc + heights[x.first][x.second] + 1;
            });

        std::cout << "Answer: " << risk << std::endl;
    }
    else
    {
        // Find and fill the basins
        std::vector<int> basinSizes;

        for (int r = 0; r < HEIGHT; ++r)
        {
            for (int c = 0; c < WIDTH; ++c)
            {
                if (heights[r][c] < 9)
                {
                    int size = fillBasin(heights, r, c);
                    basinSizes.push_back(size);
                }
            }
        }

        std::sort(basinSizes.begin(), basinSizes.end());

//        int64_t result = 1;
//        for (auto p = basinSizes.rbegin(); p != basinSizes.rbegin() + 3; ++p)
//        {
//            result *= (int64_t)*p;
//        }

        int64_t result = std::accumulate(basinSizes.rbegin(), basinSizes.rbegin() + 3, (int64_t)1, std::multiplies<int64_t>{});
        std::cout << "Product of 3 biggest basin sizes is " << result << std::endl;
    }

    return 0;
}

static HeightMap buildMap(std::vector<std::string> const & lines)
{
    HeightMap heights;
    size_t r = 0;
    for (auto const& line : lines)
    {
        if (line.size() != WIDTH)
            throw std::runtime_error("Size of a line is not equal to WIDTH.");

        for (int c = 0; c < WIDTH; ++c)
        {
            heights[r][c] = line[c] - '0';
        }
        ++r;
    }
    if (r != HEIGHT)
        throw std::runtime_error("Height of the array is not equal to HEIGHT.");

    return heights;
}

bool isLowPoint(HeightMap const& heights, int r, int c)
{
    int h = heights[r][c];

    if (r - 1 >= 0 && heights[r - 1][c] <= h)
        return false;
    if (r + 1 < HEIGHT && heights[r + 1][c] <= h)
        return false;
    if (c - 1 >= 0 && heights[r][c - 1] <= h)
        return false;
    if (c + 1 < WIDTH && heights[r][c + 1] <= h)
        return false;
    return true;
}

static int fillBasin(HeightMap& heights, int r, int c)
{
    int size = 0;

    std::deque<std::pair<int, int>> queue;
    queue.push_back(std::make_pair(r, c));

    while (!queue.empty())
    {
        std::pair<int, int> p = queue.front();
        queue.pop_front();

        int r = p.first;
        int c = p.second;

        if (heights[p.first][p.second] < 9)
        {
            heights[p.first][p.second] = 9;
            ++size;

            if (r - 1 >= 0 && heights[r - 1][c] < 9)
            {
                std::pair<int, int> adjacent{ r - 1, c };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (r + 1 < HEIGHT && heights[r + 1][c] < 9)
            {
                std::pair<int, int> adjacent{ r + 1, c };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (c - 1 >= 0 && heights[r][c - 1] < 9)
            {
                std::pair<int, int> adjacent{ r, c - 1 };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
            if (c + 1 < WIDTH && heights[r][c + 1] < 9)
            {
                std::pair<int, int> adjacent{ r, c + 1 };
                if (std::find(queue.begin(), queue.end(), adjacent) == queue.end())
                    queue.push_back(adjacent);
            }
        }
    }
    return size;
}
