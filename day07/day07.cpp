// Advent of Code 2021
// Day 7

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <numeric>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 7;

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    auto positions = load::commaSeparatedIntegers(inputPath);

    std::sort(positions.begin(), positions.end());

    if (part == 1)
    {
        int64_t best_sum = std::numeric_limits<int64_t>::max();
        int64_t best_x = 0;
        for (int64_t x = positions.front(); x <= positions.back(); ++x)
        {
            int64_t sum = std::reduce(positions.begin(), positions.end(), int64_t{0}, [x](int64_t acc, int64_t p)
            {
                return acc + std::abs(p - x);
            });
            if (sum < best_sum)
            {
                best_x = x;
                best_sum = sum;
            }
        }

        std::cout << "Answer: " << best_sum << std::endl;
    }
    else
    {
        int64_t best_sum = std::numeric_limits<int64_t>::max();
        int64_t best_x = 0;
        for (int64_t x = positions.front(); x <= positions.back(); ++x)
        {
            int64_t sum = std::reduce(positions.begin(), positions.end(), int64_t{ 0 }, [x](int64_t acc, int64_t p)
            {
                int64_t d = abs(p - x);
                return acc + d * (d + 1) / 2;
            });
            if (sum < best_sum)
            {
                best_x = x;
                best_sum = sum;
            }
        }

        std::cout << "Answer: " << best_sum << std::endl;
    }


    return 0;
}
