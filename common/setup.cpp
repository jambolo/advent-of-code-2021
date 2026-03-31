/// Miscellaneous setup for the solutions

#include <iostream>
#include <string>

#include "CLI/CLI.hpp"

#include "setup.h"


namespace setup
{

void parseCommandLine(int argc, char** argv, int day, std::string* path, int* part)
{
    // Build the default path to the input file
    std::string dayString = (day < 10) ? "0" + std::to_string(day) : std::to_string(day);
    std::string defaultPath = "day" + dayString + "-input.txt";

    CLI::App app{ "Advent of Code 2021" };
    app.add_option("-i,--i", *path, "Path to the input file")->default_val(defaultPath);
    app.add_option("-p, --part", *part, "Part of the puzzle to solve (1 or 2)")->default_val(1);
    app.parse(argc, argv);
}

void printBanner(int day, int part)
{
    std::cout << " == Advent of Code 2021 - Day " << day << ", part " << part << " ==\n";
}

} // namespace setup

