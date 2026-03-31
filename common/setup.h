#if !defined(SETUP_H_INCLUDED)
#define SETUP_H_INCLUDED
#pragma once

namespace setup
{
    //! Parses the command line arguments and fills in the path and part variables.
    void parseCommandLine(int argc, char ** argv, int day, std::string * path, int * part);

    //! Prints a banner with the day and part of the puzzle being solved.
    void printBanner(int day, int part);

} // namespace setup

#endif // !defined(SETUP_H_INCLUDED)