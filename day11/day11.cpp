// Advent of Code 2021
// Day 11

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 11;
static int constexpr NUMBER_OF_STEPS = 100;
static int constexpr WIDTH = 10;
static int constexpr HEIGHT = 10;
static int constexpr FLASHED = WIDTH * HEIGHT * WIDTH * HEIGHT + 1; // Sentinel value for cells that have flashed

using Map = std::array<std::array<int, WIDTH>, HEIGHT>;

static void bumpCell(int& cell);
static void bumpNeighbors(Map& map, int r, int c);
static void printMap(Map const & map);
static void bumpEnergy(Map& map);
static int propagateFlashes(Map& map);
static int resetFlashes(Map& map);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    // Load the input
    std::vector<std::string> lines = load::lines(inputPath);

    // Build the map
    Map map;
    for (int r = 0; r < HEIGHT; ++r)
        for (int c = 0; c < WIDTH; ++c)
            map[r][c] = lines[r][c] - '0';

    if (part == 1)
    {
        int nFlashed = 0;
        for (int step = 0; step < NUMBER_OF_STEPS; ++step)
        {
            // Bump the energy level
            bumpEnergy(map);

            // Flash
            nFlashed += propagateFlashes(map);

            // Reset flashed cells
            resetFlashes(map);
        }

        std::cout << nFlashed << " cells flashed." << std::endl;
    }
    else
    {
        int step = 1;
        while (true)
        {
            // Bump the energy level
            bumpEnergy(map);

            // Flash
            propagateFlashes(map);

            // Reset flashed cells
            int nFlashedInOneStep = resetFlashes(map);
            if (nFlashedInOneStep == WIDTH * HEIGHT)
            {
                std::cout << "Answer: " << step << std::endl;
                break;
            }
            ++step;
        }
    }

    return 0;
}

static void bumpCell(int& cell)
{
    if (cell != FLASHED)
        ++cell;
}

static void bumpNeighbors(Map& map, int r, int c)
{
    if (r > 0)
    {
        if (c > 0)
            bumpCell(map[r - 1][c - 1]);
        bumpCell(map[r - 1][c]);
        if (c < WIDTH - 1)
            bumpCell(map[r - 1][c + 1]);
    }

    if (c > 0)
        bumpCell(map[r][c - 1]);
    if (c < WIDTH - 1)
        bumpCell(map[r][c + 1]);

    if (r < HEIGHT - 1)
    {
        if (c > 0)
            bumpCell(map[r + 1][c - 1]);
        bumpCell(map[r + 1][c]);
        if (c < WIDTH - 1)
            bumpCell(map[r + 1][c + 1]);
    }
}

void printMap(Map const& map)
{
    for (int r = 0; r < HEIGHT; ++r)
    {
        std::string row(WIDTH, '.');
        for (int c = 0; c < WIDTH; ++c)        {
            int level = map[r][c];
            if (level == FLASHED)
                row[c] = '#';
            else if (level >= 10)
                row[c] = 'X';
            else
                row[c] = level + '0';
        }
        std::cerr << row << std::endl;
    }
    std::cerr << std::endl;
}

static void bumpEnergy(Map& map)
{
    for (int r = 0; r < HEIGHT; ++r)
        for (int c = 0; c < WIDTH; ++c)
            map[r][c] += 1;
}

static int propagateFlashes(Map& map)
{
    bool flashed;
    int nFlashed = 0;
    do
    {
        flashed = false;    // Nobody has flashed yet

        for (int r = 0; r < HEIGHT; ++r)
        {
            for (int c = 0; c < WIDTH; ++c)
            {
                if (map[r][c] >= 10 && map[r][c] != FLASHED)
                {
                    map[r][c] = FLASHED;
                    ++nFlashed;
                    bumpNeighbors(map, r, c);
                    flashed = true;
                }
            }
        }
    } while (flashed);

    return nFlashed;
}

static int resetFlashes(Map& map)
{
    int nFlashed = 0;
    for (int r = 0; r < HEIGHT; ++r)
    {
        for (int c = 0; c < WIDTH; ++c)
        {
            if (map[r][c] == FLASHED)
            {
                map[r][c] = 0;
                ++nFlashed;
            }
        }
    }
    return nFlashed;
}
