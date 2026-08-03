// DataRW.h - Tools for reading and writing miscellaneous objects.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <string>
#include <istream>
#include <iostream>

#include "MyFile.h"

namespace Xiaoxuan4096 {
	bool readIntInput(int& number, int minimal, int maximal, bool enterToSkip = false, std::istream& in = std::cin);
	std::string readIntInputWithExit(int& number, int minimal, int maximal, bool enterToSkip = false, std::istream& in = std::cin);
	std::string readStringInput(bool enterToSkip = false, const std::string defaultReturn = "Exit", std::istream& in = std::cin);

	int readCurrentLevel(MyFile& reader);
	void saveCurrentLevel(int currentLevel, MyFile& writer);
	int readMaximumLevel(MyFile& reader);
	void saveMaximumLevel(int maximumLevel, MyFile& writer);
}