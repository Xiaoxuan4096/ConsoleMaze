// DataRW.cpp - Tools for reading and writing miscellaneous objects.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

#include <cctype>

#include "MyFile.h"

#include "DataRW.h"

namespace Xiaoxuan4096 {
	bool readIntInput(int& number, int minimal, int maximal, bool enterToSkip, std::istream& in) {
		std::stringstream ss;
		std::string input;
		int tmp;

		std::getline(in, input);
		if (input == "" && enterToSkip)
			return true;

		ss << input;
		ss >> tmp;

		if (tmp >= minimal && tmp <= maximal) {
			number = tmp;
			return true;
		}
		return false;
	}
	std::string readIntInputWithExit(int& number, int minimal, int maximal, bool enterToSkip, std::istream& in) {
		std::stringstream ss;
		std::string input;
		int tmp = std::numeric_limits<int>::min();

		std::getline(in, input);
		if (input == "" && enterToSkip)
			return "Progress";

		for (size_t i = 0; i < input.size(); i++)
			if (isalpha(input[i]))
				input[i] = tolower(input[i]);
		if (input == "exit")
			return "Exit";

		ss << input;
		ss >> tmp;

		if (tmp >= minimal && tmp <= maximal) {
			number = tmp;
			return "Progress";
		}
		return "Fail";
	}
	std::string readStringInput(bool enterToSkip, const std::string defaultReturn, std::istream& in) {
		std::string input;

		std::getline(in, input);
		if (input == "" && enterToSkip)
			return defaultReturn;

		if (isalpha(input[0]))
			input[0] = toupper(input[0]);
		for (size_t i = 1; i < input.size(); i++)
			if (isalpha(input[i]))
				input[i] = tolower(input[i]);
		return input;
	}

	int readCurrentLevel(MyFile& reader) {
		int currentLevel = 1;
		std::stringstream ss;

		reader.linkToFile("../Levels/CurrentLevel.dat");
		ss << reader.read();
		reader.unlinkFile();

		if (ss.str() != "")
			ss >> currentLevel;
		return currentLevel;
	}
	void saveCurrentLevel(int currentLevel, MyFile& writer) {
		std::stringstream ss;
		std::string tmp;

		ss << currentLevel;
		ss >> tmp;

		writer.linkToFile("../Levels/CurrentLevel.dat");
		writer.rewrite(tmp);
		writer.unlinkFile();

		return;
	}
	int readMaximumLevel(MyFile& reader) {
		int maximumLevel = 1;
		std::stringstream ss;

		reader.linkToFile("../Levels/MaximumLevel.dat");
		ss << reader.read();
		reader.unlinkFile();

		if (ss.str() != "")
			ss >> maximumLevel;
		return maximumLevel;
	}
	void saveMaximumLevel(int maximumLevel, MyFile& writer) {
		std::stringstream ss;
		std::string tmp;

		ss << maximumLevel;
		ss >> tmp;

		writer.linkToFile("../Levels/MaximumLevel.dat");
		writer.rewrite(tmp);
		writer.unlinkFile();

		return;
	}
}