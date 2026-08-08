// Game.cpp - Game Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>
#include <chrono>
#include <sstream>
#include <limits>

#include <conio.h>

#include "MyTranslator.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"
#include "MyMatrix2D.h"

#include "Game.h"

#include "../GenerateDrawRequestData/GenerateDrawRequestData.h"
#include "../DataRW/DataRW.h"

namespace Xiaoxuan4096 {
	static MyMatrix2D readLevelMaze(int level, MyFile& reader) {
		std::stringstream ss;
		std::string levelString;
		MyMatrix2D maze;

		ss << level;
		ss >> levelString;

		reader.linkToFile("../Levels/" + levelString + "/Maze.txt");
		maze = generateMyMatrix2DFromString(reader.read());
		reader.unlinkFile();

		return maze;
	}
	static long long readLevelRecord(int level, MyFile& reader) {
		std::stringstream ss;
		std::string levelString, recordString;
		long long record;

		ss << level;
		ss >> levelString;

		reader.linkToFile("../Levels/" + levelString + "/Record.dat");
		recordString = reader.read();
		reader.unlinkFile();

		if (recordString == "")
			record = std::numeric_limits<long long>::max();
		else {
			ss.clear();
			ss << recordString;
			ss >> record;
		}

		return record;
	}
	static void saveLevelRecord(int level, long long record, MyFile& writer) {
		std::stringstream ss;
		std::string levelString, recordString;

		ss << level;
		ss >> levelString;
		ss.clear();
		ss << record;
		ss >> recordString;

		writer.linkToFile("../Levels/" + levelString + "/Record.dat");
		writer.rewrite(recordString);
		writer.unlinkFile();

		return;
	}

	static void getStartAndEnd(MyMatrix2D& maze, size_t& currentx, size_t& currenty, size_t& endx, size_t& endy) {
		for (size_t i = 0; i < maze.getRowCount(); i++)
			for (size_t j = 0; j < maze.getColCount(i); j++) {
				if (maze[i][j] == 'O') {
					currentx = i;
					currenty = j;
				}
				if (maze[i][j] == 'E') {
					endx = i;
					endy = j;
				}
			}

		return;
	}

	bool game(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW, bool exp) {
		int currentLevel = readCurrentLevel(fileRW), maximumLevel = readMaximumLevel(fileRW);
		int level = currentLevel;
		MyMatrix2D maze;
		long long recordMilliseconds;
		size_t endx = 0, endy = 0, currentx = 0, currenty = 0;

		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("SelectLevel", currentLevel, 1, currentLevel), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		std::string command = readIntInputWithExit(level, 1, currentLevel, true);
		while (command == "Fail") {
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryMenu", 1, currentLevel), 4, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("SelectLevel", currentLevel, 1, currentLevel), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			command = readIntInputWithExit(level, 1, currentLevel, true);
		}
		if (command == "Exit")
			return false;

		maze = readLevelMaze(level, fileRW);
		recordMilliseconds = readLevelRecord(level, fileRW);
		getStartAndEnd(maze, currentx, currenty, endx, endy);

		auto begin = std::chrono::steady_clock::now();
		while (currentx != endx || currenty != endy) {
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("CurrentLevel", level), 0, 0), generateDrawRequestDataFromMyMatrix2D(maze, 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			char ch = _getch();
			switch (ch) {
				case 'w':
					if (maze[currentx - 1][currenty] != '#') {
						maze[currentx--][currenty] = ' ';
						maze[currentx][currenty] = 'O';
					}
					break;
				case 'a':
					if (maze[currentx][currenty - 1] != '#') {
						maze[currentx][currenty--] = ' ';
						maze[currentx][currenty] = 'O';
					}
					break;
				case 's':
					if (currentx + 1 < maze.getRowCount() && maze[currentx + 1][currenty] != '#') {
						maze[currentx++][currenty] = ' ';
						maze[currentx][currenty] = 'O';
					}
					break;
				case 'd':
					if (currenty + 1 < maze.getColCount(currentx) && maze[currentx][currenty + 1] != '#') {
						maze[currentx][currenty++] = ' ';
						maze[currentx][currenty] = 'O';
					}
					break;
				case 'q':
					return true;
				default:
					break;
			}
		}
		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		long long seconds = duration.count() / 1000;
		short milliseconds = duration.count() % 1000;
		if (duration.count() < recordMilliseconds)
			saveLevelRecord(level, duration.count(), fileRW);

		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("CurrentLevel", level), 0, 0), generateDrawRequestDataFromString(translator.getTranslation(duration.count() >= recordMilliseconds ? "WinWithoutRecord" : "WinWithRecord", level, seconds, milliseconds), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		if (level == currentLevel && currentLevel < maximumLevel)
			saveCurrentLevel(++currentLevel, fileRW);

		return true;
	}
}