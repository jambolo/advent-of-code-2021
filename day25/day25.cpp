// Advent of Code 2021
// Day 25

#include "common/expect.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 25;

static size_t constexpr WIDTH   = 139;
static size_t constexpr HEIGHT  = 137;

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    // Read the input
    auto lines = load::lines(inputPath);

    int iterations = 0;
    int moveCount;

    do
    {
        moveCount = 0;
        std::vector<std::string> newLines = lines;

        // Move right
        for (size_t i = 0; i < HEIGHT; ++i)
        {
            std::string & line = lines[i];
            std::string& newLine = newLines[i];
            for (size_t j = 0; j < WIDTH; ++j)
            {
                size_t j1 = (j + 1) % WIDTH;
                if (line[j] == '>' && line[j1] == '.')
                {
                    newLine[j] = '.';
                    newLine[j1] = '>';
                    ++moveCount;
                }
            }
        }
        lines = newLines;

        // Move down
        for (size_t i = 0; i < HEIGHT; ++i)
        {
            std::string& line = lines[i];
            std::string& line1 = lines[(i + 1) % HEIGHT];
            std::string& newLine  = newLines[i];
            std::string& newLine1 = newLines[(i + 1) % HEIGHT];
            for (size_t j = 0; j < WIDTH; ++j)
            {
                if (line[j] == 'v' && line1[j] == '.')
                {
                    newLine[j] = '.';
                    newLine1[j] = 'v';
                    ++moveCount;
                }
            }
        }
        lines = newLines;
        ++iterations;
    } while (moveCount > 0);

    std::cout << "Answer: " << iterations << std::endl;

    return 0;
}
