// MazeGenerator.cpp - Functions for automated maze generation.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <random>
#include <string>
#include <vector>

#include "MyMatrix2D.h"

#include "MazeGenerator.h"

namespace Xiaoxuan4096 {
	enum class Directions {
		Up, Down, Left, Right, Retry
	};

	class Worm {
	public:
		size_t currentRow = 0, currentCol = 0;
		Directions currentDirection = Directions::Right;
		bool living = true;
	public:
		void dig(MyMatrix2D& maze) {
			switch (currentDirection) {
				case Directions::Up:
					if (currentRow > 0)
						maze[--currentRow][currentCol] = ' ';
					break;
				case Directions::Down:
					if (currentRow < maze.getRowCount() - 1)
						maze[++currentRow][currentCol] = ' ';
					break;
				case Directions::Left:
					if (currentCol > 0)
						maze[currentRow][--currentCol] = ' ';
					break;
				case Directions::Right:
					if (currentCol < maze.getColCount(currentRow) - 1)
						maze[currentRow][++currentCol] = ' ';
					break;
			}
			return;
		}
		void changeDirection(MyMatrix2D maze) {
			static std::mt19937 gen(std::random_device{}());
			std::uniform_int_distribution<size_t> dist(0, 3);
			Directions mapping[4] = {Directions::Up, Directions::Down, Directions::Left, Directions::Right};
			size_t nextDirection;

			if (currentRow == 0)
				mapping[0] = Directions::Retry;
			if (currentRow == maze.getRowCount() - 1)
				mapping[1] = Directions::Retry;
			if (currentCol == 0)
				mapping[2] = Directions::Retry;
			if (currentCol == maze.getColCount(currentRow) - 1)
				mapping[3] = Directions::Retry;

			do
				nextDirection = dist(gen);
			while (mapping[nextDirection] == Directions::Retry);

			currentDirection = mapping[nextDirection];
			return;
		}
		void die() {
			living = false;
			return;
		}
	};

	static void generateFullMaze(MyMatrix2D& maze, size_t rows, size_t cols) {
		std::string emptyStr('#', cols);
		for (size_t i = 0; i < rows; i++)
			maze.addRow(emptyStr);
		return;
	}
	static bool arriveBorder(Worm worm, size_t rows, size_t cols) {
		switch (worm.currentDirection) {
			case Directions::Up:
				return worm.currentRow == 0;
			case Directions::Down:
				return worm.currentRow == rows - 1;
			case Directions::Left:
				return worm.currentCol == 0;
			case Directions::Right:
				return worm.currentCol == cols - 1;
		}
	}
	static size_t generateRandomSize_t(size_t min, size_t max) {
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<size_t> dist(min, max);
		return dist(gen);
	}

	static void reproduceWorm(std::vector<Worm>& wormlist, MyMatrix2D& maze) {
		std::vector<Worm> newWormlist;
		for (const Worm& worm : wormlist) {
			if (!worm.living || generateRandomSize_t(0, 100) > 5)
				continue;
			Worm tmpWorm;
			tmpWorm.currentRow = worm.currentRow;
			tmpWorm.currentCol = worm.currentCol;
			tmpWorm.currentDirection = worm.currentDirection;
			tmpWorm.changeDirection(maze);
			newWormlist.push_back(tmpWorm);
		}
		for (const Worm& worm : newWormlist)
			wormlist.push_back(worm);
		return;
	}

	MyMatrix2D generateRandomMaze(size_t rows, size_t cols) {
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<size_t> rowdist(0, rows != 0 ? rows - 1 : 0), coldist(0, cols != 0 ? cols - 1 : 0);
		size_t startRow = rowdist(gen), startCol = coldist(gen);
		MyMatrix2D maze;
		std::vector<Worm> wormlist;
		bool terminate = false, anyAlive;
		Worm tmpWorm;

		generateFullMaze(maze, rows, cols);
		maze[startRow][startCol] = 'O';
		tmpWorm.currentRow = startRow;
		tmpWorm.currentCol = startCol;
		do
			tmpWorm.changeDirection(maze);
		while (arriveBorder(tmpWorm, rows, cols));
		wormlist.push_back(tmpWorm);

		while (!terminate) {
			anyAlive = false;
			for (Worm& x : wormlist) {
				if (!x.living)
					continue;
				anyAlive = true;
				if (arriveBorder(x, rows, cols) && (x.currentRow != startRow || x.currentCol != startCol)) {
					terminate = true;
					maze[x.currentRow][x.currentCol] = 'E';
					x.die();
					break;
				}
				size_t random = generateRandomSize_t(0, 100);
				if (random < 25)
					x.die();
				else if (random < 50)
					x.changeDirection(maze);
				else
					x.dig(maze);
			}
			if (!anyAlive)
				break;
			if (!terminate)
				reproduceWorm(wormlist, maze);
		}

		return maze;
	}
}