// Advent of Code 2021
// Day 5

#include <nlohmann/json.hpp>

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 5;

struct Point
{
    int x, y;
};

bool operator <(Point const& a, Point const& b)
{
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

bool operator ==(Point const& a, Point const& b)
{
    return a.x == b.x && a.y == b.y;
}

struct Segment
{
    Point from;
    Point to;
};

void to_json(json& j, Point const& p)
{
    j["x"] = p.x;
    j["y"] = p.y;
}

void to_json(json& j, Segment const& s)
{
    j["from"] = s.from;
    j["to"] = s.to;
}

int countIntersections(std::vector<int> const & bitmap)
{
    int count = 0;
    for (auto b : bitmap)
    {
        if (b > 1)
            ++count;
    }

    return count;
}

void drawBitmap(std::vector<int> const & bitmap, int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        std::string row(width, '.');
        for (int x = 0; x < width; ++x)
        {
            int count = bitmap[y * width + x];
            if (count > 0)
                row[x] = '0' + count;
        }
        std::cerr << row << std::endl;
    }
}

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    auto lines = load::lines(inputPath);

    std::vector<Segment> segments;
    Segment range{ { 0, 0 }, { 0, 0 } };

    for (auto const & line : lines)
    {
        Segment s;
        int count = sscanf(line.c_str(), "%d,%d -> %d,%d", &s.from.x, &s.from.y, &s.to.x, &s.to.y);
        if (count < 4)
            break;
//        s.normalize();
        segments.push_back(s);

        range.from.x = std::min(range.from.x, std::min(s.from.x, s.to.x));
        range.from.y = std::min(range.from.y, std::min(s.from.y, s.to.y));
        range.to.x = std::max(range.to.x, std::max(s.from.x, s.to.x));
        range.to.y = std::max(range.to.y, std::max(s.from.y, s.to.y));
    }

//        std::cerr << "Range is " << json(range) << std::endl;

    // Allocate the bitmap
    size_t width = range.to.x + 1;
    size_t height = range.to.y + 1;
    size_t bitmapSize = width * height;

    if (part == 1) 
    {
        std::vector<int> bitmap(bitmapSize, 0);

        // Render the aligned lines
        for (auto const& s : segments)
        {
            //        std::cerr << json(s) << std::endl;
            if (s.from.x == s.to.x || s.from.y == s.to.y)
            {
                int dx = (s.to.x > s.from.x) ? 1 : (s.to.x < s.from.x) ? -1 : 0;
                int dy = (s.to.y > s.from.y) ? 1 : (s.to.y < s.from.y) ? -1 : 0;
                int length = std::max(abs(s.to.x - s.from.x), abs(s.to.y - s.from.y)) + 1;
                int x = s.from.x;
                int y = s.from.y;
                for (int i = 0; i < length; ++i)
                {
                    ++bitmap[y * width + x];
                    x += dx;
                    y += dy;
                }
                //            drawBitmap(bitmap, width, height);
            }
        }

        int count = countIntersections(bitmap);
        std::cout << "Answer: " << count << std::endl;
    }
    else
    {
        std::vector<int> bitmap(bitmapSize, 0);

        // Render all lines
        for (auto const& s : segments)
        {
            //        std::cerr << json(s) << std::endl;
            int dx = (s.to.x > s.from.x) ? 1 : (s.to.x < s.from.x) ? -1 : 0;
            int dy = (s.to.y > s.from.y) ? 1 : (s.to.y < s.from.y) ? -1 : 0;
            int length = std::max(abs(s.to.x - s.from.x), abs(s.to.y - s.from.y)) + 1;
            int x = s.from.x;
            int y = s.from.y;
            for (int i = 0; i < length; ++i)
            {
                ++bitmap[y * width + x];
                x += dx;
                y += dy;
            }
            //        drawBitmap(bitmap, width, height);
        }

        //    drawBitmap(bitmap, width, height);
        int count = countIntersections(bitmap);
        std::cout << "Answer: " << count << std::endl;
    }

    return 0;
}
