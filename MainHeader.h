#ifndef MAINHEADER_H
#define MAINHEADER_H

// Common header pulled in by every other file in the project.
// It centralizes the standard-library includes (I/O, strings, containers,
// random numbers, timing, file streams, and algorithms) and brings the
// std namespace into scope so the rest of the program doesn't need to
// keep re-including these or writing "std::" everywhere.
#include <iostream> // cout/cin, basic console I/O
#include <iomanip> // stream formatting helpers (setw, setprecision, etc.)
#include <string> // std::string
#include <iomanip> // (duplicate include, harmless)
#include <cstdlib> // rand(), srand(), etc.
#include <ctime> // time(), used to seed rand()
#include <vector> // std::vector, used for hands/decks/collections
#include <limits> // numeric_limits, used for clearing bad cin input
#include <fstream> // file streams, used to save stats to disk
#include <algorithm> // std::sort, std::all_of, etc.
using namespace std;

#endif
