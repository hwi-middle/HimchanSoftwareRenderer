#pragma once

#ifdef NDEBUG

#define ASSERT(expression) ((void)0)

#else
#include <fstream>
#include <iostream>
#include <sstream>

#define ASSERT(expression) do {                                                 \
    if (!(expression)) {                                                        \
		std::ostringstream messageStream;                                       \
		messageStream << "Assertion failed: " << #expression << "\n";	        \
        messageStream << "at [" << __FILE__ << "] (" << __func__ << ")\n";      \
        messageStream << "line " << __LINE__ << "\n";                           \
		std::ofstream outfile("Error.log");                                     \
		std::cerr.rdbuf(outfile.rdbuf());                                       \
        std::cerr << messageStream.str();                                       \
        std::cout << messageStream.str();                                       \
        std::cout << "The log file has been saved as 'Error.log'";              \
        __debugbreak();                                                         \
    }                                                                           \
} while (0)

#endif