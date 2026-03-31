// Advent of Code 2021
// Day 15

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

#include <nlohmann/json.hpp>

#include "common/PathFinder.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 15;

static int constexpr WIDTH = 100;
static int constexpr HEIGHT = 100;
static int constexpr REPEAT = 5;

struct Day15Node : public PathFinder::Node
{
    float risk;
    int row;
    int column;

    Day15Node(int v, int r, int c) : risk(float(v)), row(r), column(c) {}
    virtual float h(Node const * goal) const override
    {
        Day15Node const* g = static_cast<Day15Node const*>(goal);
        return float(std::abs(row - g->row) + std::abs(column - g->column));
    };
};

void to_json(json& j, PathFinder::Node const* p)
{
    Day15Node const * n = static_cast<Day15Node const *>(p);

    j["risk"] = n->risk;
    j["row"] = n->row;
    j["column"] = n->column;
}

void to_json(json& j, PathFinder::Edge const* e)
{
    j["cost"] = e->cost;
    j["to"] = e->to;
}

PathFinder::Node * nodeAt(PathFinder::NodeList const & nodes, int row, int column, int width)
{
    return nodes[row * width + column];
}

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    std::vector<std::string> lines = load::lines(inputPath);

    int totalWidth = (part == 1) ? WIDTH : WIDTH * REPEAT;
    int totalHeight = (part == 1) ? HEIGHT : HEIGHT * REPEAT;

    // Create the nodes
    PathFinder::NodeList nodes;
    nodes.reserve(totalWidth * totalHeight);

    if (part == 1)
    {
        for (int r = 0; r < totalHeight; ++r)
        {
            std::string const& line = lines[r];
            for (int c = 0; c < totalWidth; ++c)
            {
                int risk = line[c] - '0';
                nodes.push_back(new Day15Node(risk, r, c));
            }
        }
    }
    else
    {
        for (int i = 0; i < REPEAT; ++i)
        {
            for (int r = 0; r < HEIGHT; ++r)
            {
                for (int j = 0; j < REPEAT; ++j)
                {
                    std::string const& line = lines[r];
                    for (int c = 0; c < WIDTH; ++c)
                    {
                        int risk = line[c] - '0';
                        int risk1 = (risk + i + j - 1) % 9 + 1;
                        nodes.push_back(new Day15Node(risk1, i * HEIGHT + r, j * WIDTH + c));
                    }
                }
            }
        }
    }

//    std::cerr << json(nodes) << std::endl;

    // Add the edges

    for (int r = 0; r < totalHeight; ++r)
    {
        for (int c = 0; c < totalWidth; ++c)
        {
            Day15Node* n0 = static_cast<Day15Node*>(nodeAt(nodes, r, c, totalWidth));
            n0->edges.reserve(4);

            if (r > 0)
            {
                Day15Node* above = static_cast<Day15Node*>(nodeAt(nodes, r - 1, c, totalWidth));
                n0->edges.push_back(new PathFinder::Edge{ above->risk, above });
            }
            if (c > 0)
            {
                Day15Node* left = static_cast<Day15Node*>(nodeAt(nodes, r, c - 1, totalWidth));
                n0->edges.push_back(new PathFinder::Edge{ left->risk, left });
            }
            if (c < totalWidth - 1)
            {
                Day15Node* right = static_cast<Day15Node*>(nodeAt(nodes, r, c + 1, totalWidth));
                n0->edges.push_back(new PathFinder::Edge{ right->risk, right });
            }
            if (r < totalHeight - 1)
            {
                Day15Node* below = static_cast<Day15Node*>(nodeAt(nodes, r + 1, c, totalWidth));
                n0->edges.push_back(new PathFinder::Edge{ below->risk, below });
            }
        }
    }

    // Find the lowest cost path from START to GOAL

    int const START = 0;
    int const GOAL = totalWidth * totalHeight - 1;
    PathFinder pathfinder(&nodes);
    PathFinder::Path path = pathfinder.findPath(nodes[START], nodes[GOAL]);

    float risk = std::accumulate(path.begin() + 1, path.end(), 0.0f, [](float acc, PathFinder::Node* p)
        {
            return acc + static_cast<Day15Node*>(p)->risk;
        });

    std::cout << "Answer: " << risk << std::endl;

    return 0;
}
