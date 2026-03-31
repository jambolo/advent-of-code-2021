#include "load.h"

#include <fstream>
#include <stdexcept>
#include <string>

namespace load
{

std::vector<int64_t> integers(std::string_view path)
{
	std::ifstream input{std::string(path)};
	if (!input)
		throw std::runtime_error("Unable to open input file: " + std::string(path));

	std::vector<int64_t> values;

	for (int64_t value; input >> value; )
		values.push_back(value);

	if (!input.eof())
		throw std::runtime_error("Invalid numeric data in input file: " + std::string(path));

	return values;
}

std::vector<std::string> lines(std::string_view path)
{
	std::ifstream input{ std::string(path) };
	if (!input)
		throw std::runtime_error("Unable to open input file: " + std::string(path));

	std::vector<std::string> lines;

    // Read lines until EOF
    for (std::string line; std::getline(input, line); )
        lines.emplace_back(line);

	if (!input.eof())
		throw std::runtime_error("Invalid numeric data in input file: " + std::string(path));

	return lines;
}

std::vector<int64_t> commaSeparatedIntegers(std::string_view path)
{
	std::ifstream input{ std::string(path) };
	if (!input)
		throw std::runtime_error("Unable to open input file: " + std::string(path));

	std::vector<int64_t> values;

    // Read comma-separated integers until EOF
	for (int64_t value; input >> value; )
	{
		values.push_back(value);
		// Skip the comma after the number, if present
		if (input.peek() == ',')
			input.get();
    }

	return values;
}

std::string string(std::string_view path)
{
	std::ifstream input{ std::string(path) };
	if (!input)
		throw std::runtime_error("Unable to open input file: " + std::string(path));

	std::string s;

	// Read the entire file into a string
    s.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());

	return s;
}

} // namespace load
