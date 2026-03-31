// Advent of Code 2021
// Day 10

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

static int constexpr DAY = 10;

std::map<char, int64_t> corruptionPoints{ { ')', 3 }, { ']', 57 }, { '}', 1197 }, { '>', 25137 } };
std::map<char, int64_t> completionPoints{ { '(', 1 }, { '[', 2 }, { '{', 3 }, { '<', 4 } };

static int64_t checkCorruption(std::string const& line, std::vector<char>& scopes);
static int64_t checkCompletion(std::vector<char>& scopes);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    auto lines = load::lines(inputPath);

    if (part == 1)
    {
        int64_t corruptionScore = 0;
        std::vector<int64_t> completionScores;
        std::vector<char> scopes;

        for (auto const& line : lines)
        {
            scopes.clear();
            int64_t corruption = checkCorruption(line, scopes);
            corruptionScore += corruption;
        }
        std::cout << "Answer: " << corruptionScore << std::endl;
    }
    else
    {
        int64_t corruptionScore = 0;
        std::vector<int64_t> completionScores;
        std::vector<char> scopes;

        for (auto const& line : lines)
        {
            scopes.clear();
            int64_t corruption = checkCorruption(line, scopes);
            if (corruption == 0)
            {
                int64_t completion = checkCompletion(scopes);
                completionScores.push_back(completion);
            }
        }
        std::sort(completionScores.begin(), completionScores.end());
        std::cout << "Completion score is " << completionScores[completionScores.size() / 2] << std::endl;
    }
    return 0;
}

static int64_t checkCorruption(std::string const & line, std::vector<char> & scopes)
{
    int64_t score = 0;
    bool error = false;

    for (char c : line)
    {
        switch (c)
        {
        case '(':
        case '[':
        case '{':
        case '<':
            scopes.push_back(c);
            break;
        case ')':
            if (scopes.back() != '(')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case ']':
            if (scopes.back() != '[')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case '}':
            if (scopes.back() != '{')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        case '>':
            if (scopes.back() != '<')
            {
                score += corruptionPoints[c];
                error = true;
            }
            else
            {
                scopes.pop_back();
            }
            break;
        default:
            throw std::runtime_error(std::string("Invalid input character: '") + c + "'.");
        }
        if (error)
            break;
    }
    return score;
}

static int64_t checkCompletion(std::vector<char>& scopes)
{
    int64_t score = 0;
    for (auto p = scopes.rbegin(); p != scopes.rend(); ++p)
    {
        score = score * 5 + completionPoints[*p];
    }

    return score;
}