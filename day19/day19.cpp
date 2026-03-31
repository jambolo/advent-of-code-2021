// Advent of Code 2021
// Day 19

#include "common/expect.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 19;
static int constexpr NUMBER_OF_ORIENTATIONS = 24;

using V3i = std::array<int, 3>;
using M33i = std::array<V3i, 3>;

struct Scanner
{
    std::set<V3i> beacons;
    std::vector<int> distances;
    std::vector<V3i> offsets;
    V3i offset = { 0, 0, 0 };

    void generateAnalysisData();
};

static int loadScanner(Scanner & scanner, std::vector<std::string> const& lines, int i);
static std::pair<size_t, int> chooseScanner(Scanner const& s0, std::vector<size_t> const & excluded);
static V3i combine(Scanner& combined, Scanner const& s1);
static V3i determineScannerOffset(std::set<V3i> const& b0, std::vector<V3i> const& b1);
static std::pair<int, int> chooseOrientation(Scanner const& s0, Scanner const& s1);
static std::vector<V3i> orient(std::vector<V3i> points, int i);
static V3i orient(V3i const & v, int i);
static int countDistanceMatches(std::vector<int> const & d1, std::vector<int> const & d0);
static int countBeaconMatches(std::set<V3i> const& beacons, std::vector<V3i> const& otherBeacons);
static int countOffsetMatches(std::vector<V3i> const& setOffsets, std::vector<V3i> const& otherOffsets);

static std::vector<Scanner> scanners;

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    // Read the input
    auto lines = load::lines(inputPath);

    // Initialize scanners
    bool ok = true;
    int i = 0;
    while (i < lines.size())
    {
        Scanner scanner;
        i = loadScanner(scanner, lines, i);
        scanner.generateAnalysisData();
        scanners.emplace_back(std::move(scanner));
    }

    std::pair<size_t, size_t> initial = { 14, 17 };// chooseInitialScannerPair();

    Scanner combined = scanners[initial.first];
    V3i offset = combine(combined, scanners[initial.second]);
    scanners[initial.second].offset = offset;

    std::vector<size_t> excluded = { initial.first, initial.second };
    while (excluded.size() < scanners.size())
    {
        std::pair<size_t, int> match = chooseScanner(combined, excluded);
        V3i offset = combine(combined, scanners[match.first]);
        scanners[match.first].offset = offset;
        excluded.push_back(match.first);
    }

    if (part == 1)
    {
        std::cout << "Answer: " << combined.beacons.size() << std::endl;
    }
    else
    {
        int maxDistance = 0;
        for (int i = 1; i < scanners.size(); ++i)
        {
            V3i const& oi = scanners[i].offset;
            for (int j = 0; j < i; ++j)
            {
                V3i const& oj = scanners[j].offset;
                int distance = std::abs(oi[0] - oj[0]) + std::abs(oi[1] - oj[1]) + std::abs(oi[2] - oj[2]);
                if (distance > maxDistance)
                    maxDistance = distance;
            }
        }
        std::cout << "Answer: " << maxDistance << std::endl;
    }

    return 0;
}

static int loadScanner(Scanner & scanner, std::vector<std::string> const& lines, int i)
{
    std::string const & line = lines[i++];
    std::regex header_pattern("--- scanner ([0-9]+) ---");
    std::smatch match;
    bool found = std::regex_match(line, match, header_pattern);
    if (!found)
        throw std::runtime_error("regex failed");

    while (i < lines.size() && !lines[i].empty())
    {
        std::istringstream lineStream(lines[i++]);
        int x, y, z;
        lineStream >> x >> expect(',') >> y >> expect(',') >> z;
        scanner.beacons.emplace(V3i{ x, y, z });
    }
    return ++i;
}

static std::pair<size_t, int> chooseScanner(Scanner const& s0, std::vector<size_t> const & excluded)
{
    std::vector<int> const& d0 = s0.distances;

    int best = 0;
    size_t bestIndex;

    for (size_t j = 0; j < scanners.size(); ++j)
    {
        if (std::find(excluded.begin(), excluded.end(), j) != excluded.end())
            continue;

        Scanner const& s1 = scanners[j];
        std::vector<int> const& d1 = s1.distances;

        int matches =  countDistanceMatches(d0, d1);
        if (matches > best)
        {
            best = matches;
            bestIndex = j;
        }
    }
    return std::make_pair(bestIndex, best);
}

static V3i combine(Scanner& s0, Scanner const& s1)
{
    // Find the best orientation
    std::pair<int, int> best = chooseOrientation(s0, s1);

    // Determine the location of s1 relative to s0
    std::vector<V3i> orientedS1 = orient(std::vector<V3i>(s1.beacons.begin(), s1.beacons.end()), best.first);
    V3i offset = determineScannerOffset(s0.beacons, orientedS1);

    // Offset s1 beacons relative to s0
    for (auto& v : orientedS1)
    {
        v[0] -= offset[0];
        v[1] -= offset[1];
        v[2] -= offset[2];
    }

    // Add non-duplicated beacons to s0
    s0.beacons.insert(orientedS1.begin(), orientedS1.end());

    // Update the analysis data
    s0.generateAnalysisData();

    return offset;
}

static V3i determineScannerOffset(std::set<V3i> const & beacons0, std::vector<V3i> const & beacons1)
{
    // Count all the possible offsets
    std::map<V3i, int> possibleOffsets;
    for (auto const & b0 : beacons0)
    {
        for (auto const& b1 : beacons1)
        {
            ++possibleOffsets[V3i{b1[0] - b0[0], b1[1] - b0[1], b1[2] - b0[2]}];
        }
    }

    // Find the one that occurs the most and go with that
    V3i bestOffset{ 0, 0, 0 };
    int best = 0;
    for (auto const& e : possibleOffsets)
    {
        if (e.second > best)
        {
            best = e.second;
            bestOffset = e.first;
        }
    }
    return bestOffset;
}

static std::pair<int, int> chooseOrientation(Scanner const & s0, Scanner const & s1)
{
    std::vector<V3i> const& o0 = s0.offsets;
    std::vector<V3i>const& o1 = s1.offsets;
    int bestOrientation = -1;
    int best = 0;
    for (int orientation = 0; orientation < NUMBER_OF_ORIENTATIONS; ++orientation)
    {
        std::vector<V3i> orientedOffsets = orient(o1, orientation);
        int matches = countOffsetMatches(o0, orientedOffsets);
        if (matches > best)
        {
            best = matches;
            bestOrientation = orientation;
        }
    }
    return std::make_pair(bestOrientation, best);
}

std::vector<V3i> orient(std::vector<V3i> points, int i)
{
    for (auto& p : points)
        p = orient(p, i);
    return points;
}

static V3i orient(V3i const& v, int i)
{
    static std::array<V3i, 24> mapping{ {
        { 0, 1, 2 }, { 0, 2, 1 }, { 0, 1, 2 }, { 0, 2, 1 },
        { 1, 0, 2 }, { 1, 2, 0 }, { 1, 0, 2 }, { 1, 2, 0 },
        { 2, 0, 1 }, { 2, 1, 0 }, { 2, 0, 1 }, { 2, 1, 0 },
        { 0, 1, 2 }, { 0, 2, 1 }, { 0, 1, 2 }, { 0, 2, 1 },
        { 1, 0, 2 }, { 1, 2, 0 }, { 1, 0, 2 }, { 1, 2, 0 },
        { 2, 0, 1 }, { 2, 1, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
    } };

    static std::array<V3i, 24> signs{ {
        {  1,  1,  1 }, {  1,  1, -1 }, {  1, -1, -1 }, {  1, -1,  1 },
        {  1,  1, -1 }, {  1,  1,  1 }, {  1, -1,  1 }, {  1, -1, -1 },
        {  1,  1,  1 }, {  1,  1, -1 }, {  1, -1, -1 }, {  1, -1,  1 },
        { -1,  1, -1 }, { -1,  1,  1 }, { -1, -1,  1 }, { -1, -1, -1 },
        { -1,  1,  1 }, { -1,  1, -1 }, { -1, -1, -1 }, { -1, -1,  1 },
        { -1,  1, -1 }, { -1,  1,  1 }, { -1, -1,  1 }, { -1, -1, -1 }
    } };

    return V3i
    {
        v[mapping[i][0]] * signs[i][0],
        v[mapping[i][1]] * signs[i][1],
        v[mapping[i][2]] * signs[i][2]
    };
}

static int countDistanceMatches(std::vector<int> const & d0, std::vector<int> const & d1)
{
    int matches = 0;
    auto lb1 = d1.begin();
    for (auto d : d0)
    {
        lb1 = std::lower_bound(lb1, d1.end(), d);
        if (lb1 != d1.end() && *lb1 == d)
            ++matches;
    }
    return matches;
}

static int countBeaconMatches(std::set<V3i> const & beacons, std::vector<V3i> const & otherBeacons)
{
    int matches = 0;
    auto lb1 = otherBeacons.begin();
    for (auto const& b : beacons)
    {
        lb1 = std::lower_bound(lb1, otherBeacons.end(), b);
        if (lb1 != otherBeacons.end() && *lb1 == b)
            ++matches;
    }
    return matches;
}

static int countOffsetMatches(std::vector<V3i> const& scannerOffsets, std::vector<V3i> const& otherOffsets)
{
    int matches = 0;
    for (auto const & v : otherOffsets)
    {
        if (std::binary_search(scannerOffsets.begin(), scannerOffsets.end(), v))
            ++matches;
        else if (std::binary_search(scannerOffsets.begin(), scannerOffsets.end(), V3i{ -v[0], -v[1], -v[2] }))
            ++matches;
    }
    return matches;
}

void Scanner::generateAnalysisData()
{
    distances.clear();
    offsets.clear();
    std::vector<V3i> b(beacons.begin(), beacons.end()); // Need a random access iterator to generate all combinations
    for (size_t i = 1; i < b.size(); ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            V3i offset{ b[j][0] - b[i][0], b[j][1] - b[i][1], b[j][2] - b[i][2] };
            int distance = std::abs(offset[0]) + std::abs(offset[1]) + std::abs(offset[2]); // Manhattan distance is sufficient
            distances.push_back(distance);
            offsets.emplace_back(std::move(offset));
        }
    }
    std::sort(distances.begin(), distances.end());
    std::sort(offsets.begin(), offsets.end());
}
