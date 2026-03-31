// Advent of Code 2021
// Day 6

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

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 6;
static int constexpr NUMBER_OF_AGES = 9;    // 0 - 8

struct extract
{
    char c;
    extract(char c) : c(c) { }
};

std::istream& operator >> (std::istream& in, extract e)
{
    // Skip leading whitespace IFF user is not asking to extract a whitespace character
    if (!std::isspace(e.c))
        in >> std::ws;

    // Attempt to get the specific character
    if (in.peek() == e.c)
        in.get();
    else
        in.setstate(std::ios::failbit);

    return in;
}

static void loadAges(std::ifstream& input, std::vector<int64_t>& byAge);
static void update(int day, std::vector<int64_t>& byAge);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    std::ifstream input(inputPath);
    if (!input.is_open())
        exit(1);

    // Load picked numbers

    auto ages = load::commaSeparatedIntegers(inputPath);
    std::vector<int64_t> byAge(NUMBER_OF_AGES, 0);
    for (auto age : ages)
    {
        ++byAge[age];
    }

    if (part == 1)
    {
        int const NUMBER_OF_DAYS = 80;
        for (int day = 0; day < NUMBER_OF_DAYS; ++day)
        {
            update(day, byAge);
        }

        std::cout << "Answer: " << std::accumulate(byAge.begin(), byAge.end(), int64_t(0)) << std::endl;
    }
    else
    {
        int const NUMBER_OF_DAYS = 256;
        for (int day = 0; day < NUMBER_OF_DAYS; ++day)
        {
            update(day, byAge);
        }

        std::cout << "Answer: " << std::accumulate(byAge.begin(), byAge.end(), int64_t(0)) << std::endl;
    }

    return 0;
}

static void loadAges(std::ifstream & input, std::vector<int64_t>& byAge)
{
    std::string line;
    std::getline(input, line);
    if (input.fail())
        return;

    std::istringstream lineStream(line);
    while (!lineStream.fail())
    {
        int age;
        lineStream >> age;
        if (!lineStream.fail())
            ++byAge[age];
        lineStream >> extract(',');
    }
}

static void update(int day, std::vector<int64_t>& byAge)
{
    // Which group of fish is giving birth today?
    int iBirth = (day + 0) % byAge.size();

    // Decrement
    int iReset = (day + 7) % byAge.size();

    int64_t nBirths = byAge[iBirth];
    byAge[iReset] += nBirths;
}
