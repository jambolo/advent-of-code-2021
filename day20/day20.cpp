// Advent of Code 2021
// Day 20

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "common/expect.h"
#include "common/load.h"
#include "common/setup.h"

using json = nlohmann::json;

static int constexpr DAY = 20;

static int constexpr BORDER = 3;

using Image = std::vector<std::vector<char>>;

static Image expand(Image const& image, int n);
static Image shrink(Image const& image, int n);
static Image map(Image const& image, std::string const & mapping);
static int mapPixel(Image const& image, std::string const& mapping, int r, int c);
static int64_t countPixels(Image const& image);

int main(int argc, char** argv)
{
    std::string inputPath;
    int part;

    setup::parseCommandLine(argc, argv, DAY, &inputPath, &part);
    setup::printBanner(DAY, part);

    // Read the input
    auto lines = load::lines(inputPath);

    std::string mapping = lines[0];

    Image image;
    image.reserve(lines.size() - 2);
    for (int i = 2; i < lines.size(); ++i)
    {
        image.emplace_back(std::vector<char>(lines[i].begin(), lines[i].end()));
    }
    
    int const ITERATIONS = (part == 1) ? 2 : 50;

    image = expand(image, BORDER * ITERATIONS);
    for (int i = 0; i < ITERATIONS; ++i)
    {
        image = map(image, mapping);
    }
    image = shrink(image, BORDER * ITERATIONS / 2);
    int64_t count = countPixels(image);
    std::cout << "Answer: " << count << std::endl;

    return 0;
}

static Image expand(Image const& image, int n)
{
    Image newImage = image;
    std::vector<char> empty(image[0].size() + 2 * n, '.');

    // Expand the left side
    for (auto& row : newImage)
    {
        row.insert(row.begin(), n, '.');
        row.insert(row.end(), n, '.');
    }
    newImage.insert(newImage.begin(), n, empty);
    newImage.insert(newImage.end(), n, empty);

    return newImage;
}

static Image shrink(Image const& image, int n)
{
    Image newImage;
    newImage.reserve(image.size() - 2 * n);

    for (int i = n; i < image.size() - n; ++i)
    {
        newImage.emplace_back(std::vector<char>(image[i].begin() + n, image[i].end() - n));
    }

    return newImage;
}

static Image map(Image const& image, std::string const& mapping)
{
    size_t width = image[0].size();
    size_t height = image.size();
    std::vector<char> empty(width, '.');

    Image newImage(height);
    newImage.front() = empty;
    for (int r = 1; r < height - 1; ++r)
    {
        auto & row = newImage[r];
        row.reserve(width);
        row.push_back('.');
        for (int c = 1; c < width - 1; ++c)
        {
            row.push_back(mapPixel(image, mapping, r, c));
        }
        row.push_back('.');
    }
    newImage.back() = empty;

    return newImage;
}

static int mapPixel(Image const& image, std::string const& mapping, int r, int c)
{
    int index = 0;
    index = (index << 1) + (image[r - 1][c - 1] == '#');
    index = (index << 1) + (image[r - 1][c + 0] == '#');
    index = (index << 1) + (image[r - 1][c + 1] == '#');
    index = (index << 1) + (image[r + 0][c - 1] == '#');
    index = (index << 1) + (image[r + 0][c + 0] == '#');
    index = (index << 1) + (image[r + 0][c + 1] == '#');
    index = (index << 1) + (image[r + 1][c - 1] == '#');
    index = (index << 1) + (image[r + 1][c + 0] == '#');
    index = (index << 1) + (image[r + 1][c + 1] == '#');

    return mapping[index];
}

static int64_t countPixels(Image const& image)
{
    int64_t count = 0;
    for (auto const& row : image)
    {
        count += std::count(row.begin(), row.end(), '#');
    }

    return count;
}
