// Advent of Code 2021
// Day 18

#include <nlohmann/json.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 18;

struct Node;
using Number = std::shared_ptr<Node>;
using Path = std::vector<Number>;
using NumberList = std::vector<Number>;

struct Node
{
    int64_t v;
    Number left;
    Number right;

    bool isNumber() const { return !left || !right; }
    bool isPair() const { return !isNumber(); }
};

static void to_json(json& j, Node const& node);
static Number createNode(json const& j);
static Number clone(Number const& n);
static void reduce(Number & n);
static bool explode(Number& n);
static bool split(Number& n);
static Number leftMost(Path& path);
static Number rightMost(Path& path);
static Number next(Path& path);
static Number previous(Path& path);
static int64_t magnitude(Number& n);
Number operator +(Number const& a, Number const& b);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    // Read the input
    auto lines = load::lines(inputPath);

    // Load the numbers (clever: use a JSON parser to parse the input)
    NumberList numbers;
    for (auto const& line : lines)
    {
        json j = json::parse(line);
        Number n = createNode(j);
        numbers.push_back(n);
    }

    if (part == 1)
    {
        Number sum = std::accumulate(numbers.begin() + 1, numbers.end(), numbers[0], [](Number const& a, Number const& b) { return a + b; });
        std::cout << "Answer: " << magnitude(sum) << std::endl;
    }
    else
    {
        int64_t maxM = 0;
        for (int i = 0; i < numbers.size(); ++i)
        {
            for (int j = 0; j < numbers.size(); ++j)
            {
                if (i != j)
                {
                    Number sum = numbers[i] + numbers[j];
                    int64_t m = magnitude(sum);
                    maxM = std::max(maxM, m);
                }
            }
        }

        std::cout << "Answer: " << maxM << std::endl;
    }

    return 0;
}

static void to_json(json& j, Node const& node)
{
    if (node.isPair())
        j = json::array({ *node.left, *node.right });
    else
        j = node.v;
}

static Number createNode(json const& j)
{
    Number node(new Node);

    if (j.is_array())
    {
        node->left = createNode(j[0]);
        node->right = createNode(j[1]);
        node->v = 0;
    }
    else
    {
        node->left = nullptr;
        node->right = nullptr;
        node->v = j;
    }

    return node;
}

Number clone(Number const& n)
{
    return Number(new Node{ n->v, n->left ? clone(n->left) : Number(), n->right ? clone(n->right) : Number() });
}

static void reduce(Number & n)
{
    bool altered;
    do
    {
        altered = false;
        if (explode(n))
        {
            altered = true;
            continue;
        }
        else if (split(n))
        {
            altered = true;
            continue;
        }
    } while (altered);
}

static bool explode(Number & n)
{
    Path path{ n };
    Number x = leftMost(path);  // x is a leaf node so it is a number
    while (x)
    {
        if (path.size() >  4 + 1)
        {
            Number pair = path[path.size() - 2];    // Pair of numbers (potentially) with x on the left or right
            if (pair->left && pair->left->isNumber() && pair->right && pair->right->isNumber())
            {
                path.pop_back();    // pair is at the back now
                {
                    Path leftPath = path;
                    Number left = previous(leftPath);
                    if (left)
                        left->v += pair->left->v;
                }
                {
                    Path rightPath(path);
                    Number right = next(rightPath);
                    if (right)
                        right->v += pair->right->v;
                }

                // Link the revised pair
                path.pop_back();    // Pair's parent is at the back now
                if (path.back()->left == pair)
                    path.back()->left = Number(new Node{ 0, Number(), Number() });
                if (path.back()->right == pair)
                    path.back()->right = Number(new Node{ 0, Number(), Number() });
                return true;
            }
        }
        x = next(path);
    }
    return false;
}

static bool split(Number& n)
{
    Path path{ n };
    Number x = leftMost(path);
    while (x)
    {
        if (x->v >= 10)
        {
            x->left = Number(new Node{ x->v / 2, Number(), Number() });
            x->right = Number(new Node{ (x->v + 1) / 2, Number(), Number() });
            return true;
        }
        x = next(path);
    }
    return false;
}

static Number leftMost(Path& path)
{
    while (path.back()->left)
        path.push_back(path.back()->left);
    return path.back();
}

static Number rightMost(Path& path)
{
    while (path.back()->right)
        path.push_back(path.back()->right);
    return path.back();
}

static Number next(Path& path)
{
    Number child;
    do
    {
        child = path.back();
        path.pop_back();
    } while (!path.empty() && child == path.back()->right);

    if (path.empty())
        return Number();

    path.push_back(path.back()->right);
    return leftMost(path);
}


static Number previous(Path& path)
{
    Number n;
    do
    {
        n = path.back();
        path.pop_back();
    } while (!path.empty() && n == path.back()->left);

    if (path.empty())
        return Number();

    path.push_back(path.back()->left);
    return rightMost(path);
}

int64_t magnitude(Number& n)
{
    if (n->isPair())
        return 3 * magnitude(n->left) + 2 * magnitude(n->right);
    else
        return n->v;
}

Number operator +(Number const& a, Number const& b)
{
    Number sum(new Node{ 0, clone(a), clone(b) });
    reduce(sum);
    return sum;
}
