// Advent of Code 2021
// Day 13

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 13;
static char constexpr DOT = '#';
struct Point
{
    int r, c;
};

void to_json(json& j, Point const& p)
{
    j = std::vector<int>{ p.c, p.r };
}
using Paper = std::vector<std::string>;

static void drawPaper(Paper const& paper);
static void fold(Paper& paper, Point const& p);
static int countDots(Paper const& paper);

static std::vector<Point> folds;
static Paper paper;

static int minC = 0;
static int minR = 0;
static int maxC = std::numeric_limits<int>::min();
static int maxR = std::numeric_limits<int>::min();
static int width, height;

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    static std::vector<Point> points;

    std::vector<std::string> lines = load::lines(inputPath);

    // Load dots
    int i = 0;
    while (i < lines.size())
    {
        int r, c;

        // Look for empty line separating dots and folds
        if (lines[i].empty())
        {
            ++i;
            break;
        }
        std::istringstream lineStream(lines[i]);
        lineStream >> c >> expect(',') >> r;
        ++i;
        if (lineStream.fail())
            break;

        maxC = std::max(c, maxC);
        maxR = std::max(r, maxR);

        points.emplace_back(Point{ r, c });
    }

    // Load folds
    while (i < lines.size())
    {
        if (lines[i].empty())
        {
            ++i;
            break;
        }

        char direction;
        int location;

        // "fold along "
        std::istringstream lineStream(lines[i].substr(11));
        ++i;
        lineStream >> direction >> expect('=') >> location;
        if (lineStream.fail())
            break;

        if (direction == 'x')
            folds.push_back(Point{ 0, location });  // fold along column x
        else if (direction == 'y')
            folds.push_back(Point{ location, 0 });  // fold along row y
        else
            throw std::runtime_error("Invalid fold direction");
    }

    width = maxC - minC + 1;
    height = maxR - minR + 1;

    // Create the paper
    paper.reserve(height);
    for (int i = 0; i < height; ++i)
    {
        paper.push_back(std::string(width, ' '));
    }

    // Place the dots
    for (auto const& p : points)
        paper[p.r][p.c] = DOT;

    if (part == 1)
    {
        fold(paper, folds[0]);
        std::cout << "Answer: " << countDots(paper) << std::endl;
    }
    else
    {
        for (auto const & f : folds)
            fold(paper, f);

        drawPaper(paper);
    }

    return 0;
}

static void drawPaper(Paper const& paper)
{
    std::cout << std::endl;
    for (auto const& row : paper)
    {
        std::cout << ' ' << row << std::endl;
    }
}

static void fold(Paper& paper, Point const& p)
{
    if (p.r == 0)
    {
        if (p.c < width / 2)
            throw std::runtime_error("fold is less than half");
        for (int c = 1; c < width - p.c; ++c)
        {
            for (int r = 0; r < height; ++r)
            {
                if (paper[r][p.c + c] == DOT)
                    paper[r][p.c - c] = DOT;
            }
        }
        for (auto & row : paper)
            row.resize(p.c);
        width = p.c;
    }
    else if (p.c == 0)
    {
        if (p.r < height / 2)
            throw std::runtime_error("fold is less than half");
        for (int r = 1; r < height - p.r; ++r)
        {
            for (int c = 0; c < width; ++c)
            {
                if (paper[p.r + r][c] == DOT)
                paper[p.r - r][c] = DOT;
            }
        }
        paper.resize(p.r);
        height = p.r;

    }
    else
        throw std::runtime_error("invalid fold");
}

static int countDots(Paper const& paper)
{
    int count = 0;
    for (auto const& row : paper)
        count += (int)std::count(row.begin(), row.end(), DOT);
    return count;
}
