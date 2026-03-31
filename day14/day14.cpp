// Advent of Code 2021
// Day 14

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"


using json = nlohmann::json;

static int constexpr DAY = 14;
static int constexpr NUMBER_OF_STEPS = 40;

using Pair = std::pair<char, char>;
using CountsBySymbol = std::map<char, int64_t>;
using CountsByPair = std::map<Pair, int64_t>;
using Rules = std::map<Pair, char>;

static void doInsertions(Rules const& rules, CountsByPair& pairCounts, CountsBySymbol& symbolCounts);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    std::vector<std::string> lines = load::lines(inputPath);

    // Get the template
    std::string start = lines[0];

    Rules rules;
    for (int i = 2; i < lines.size(); ++i)
    {
        std::string const& line = lines[i];
        rules[Pair(line[0], line[1])] = line[6];
    }

    CountsByPair pairCounts;
    CountsBySymbol symbolCounts;

    // Count all of the symbols in the starting template
    for (char c : start)
        ++symbolCounts[c];

    // Count all of the valid pairs in the starting template
    for (size_t i = 0; i < start.size() - 1; ++i)
    {
        Pair pair = std::make_pair(start[i], start[i + 1]);
        auto pRule = rules.find(pair);
        if (pRule != rules.end())
            ++pairCounts[pair];
    }

    if (part == 1)
    {
        static int constexpr NUMBER_OF_STEPS = 10;
        for (int i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            doInsertions(rules, pairCounts, symbolCounts);
        }
    }
    else
    {
        static int constexpr NUMBER_OF_STEPS = 40;
        for (int i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            doInsertions(rules, pairCounts, symbolCounts);
        }
    }

    std::vector<int64_t> sorted_counts;
    sorted_counts.reserve(symbolCounts.size());
    for (auto & entry : symbolCounts)
        sorted_counts.push_back(entry.second);
    std::sort(sorted_counts.begin(), sorted_counts.end());

    int64_t result = sorted_counts.back() - sorted_counts.front();
    std::cout << "Answer: " << result << std::endl;

    return 0;
}


static void doInsertions(Rules const & rules, CountsByPair & pairCounts, CountsBySymbol & symbolCounts)
{
    std::map<Pair, int64_t> newCounts;
    for (auto & entry : pairCounts)
    {
        auto const& pair = entry.first;
        auto count = entry.second;

        // Find the rule for this pair in order to do the insertions.
        auto pRule = rules.find(pair);

        // The pair is being split up, so its count is reset to 0.
        pairCounts[pair] = 0;

        // If there is a rule for the left pair, then its count is added. Otherwise, it is ignored.
        Pair leftPair = std::make_pair(pair.first, pRule->second);
        auto pLeftRule = rules.find(leftPair);
        if (pLeftRule != rules.end())
            newCounts[leftPair] += count;

        // If there is a rule for the right pair, then its count is added. Otherwise, it is ignored.
        Pair rightPair = std::make_pair(pRule->second, pair.second);
        auto pRightRule = rules.find(rightPair);
        if (pRightRule != rules.end())
            newCounts[rightPair] += count;

        // The symbol being inserted is counted.
        symbolCounts[pRule->second] += count;
    }

    // Update the pair counts with the new counts.
    for (auto & entry : newCounts)
        pairCounts[entry.first] += entry.second;
}