// Input loading functions

#if !defined(LOAD_H_INCLUDED)
#define LOAD_H_INCLUDED 1
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace load
{
    //! Loads a list of numbers from the input file, either separated by whitespace or newlines.
	std::vector<int64_t> integers(std::string_view path);

    //! Loads lines of text from the input file.
    std::vector<std::string> lines(std::string_view path);

    //! Loads a list of numbers from the input file, either separated by whitespace or newlines.
    std::vector<int64_t> commaSeparatedIntegers(std::string_view path);

    //! Loads the input file as a single string.
    std::string string(std::string_view path);

} // namespace load


#endif // !defined(LOAD_H_INCLUDED)