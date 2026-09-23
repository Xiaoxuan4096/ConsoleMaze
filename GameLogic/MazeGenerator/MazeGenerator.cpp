// MazeGenerator.cpp - Functions for automated maze generation.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <random>
#include <string>

#include "MyMatrix2D.h"

#include "MazeGenerator.h"

namespace Xiaoxuan4096 {
	static enum class Directions {
		Up, Down, Left, Right, Retry
	};

	class Worm {
	private:
		size_t currentRow, currentCol;
		Directions currentDirection;
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

			switch (currentDirection) {
				case Directions::Up:
					if (currentRow == 0)
						mapping[0] = Directions::Retry;
					break;
				case Directions::Down:
					if (currentRow == maze.getRowCount() - 1)
						mapping[1] = Directions::Retry;
					break;
				case Directions::Left:
					if (currentCol == 0)
						mapping[2] = Directions::Retry;
					break;
				case Directions::Right:
					if (currentCol == maze.getColCount(currentRow) - 1)
						mapping[3] = Directions::Retry;
					break;
			}

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

	MyMatrix2D generateRandomMaze(size_t rows, size_t cols) {
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<size_t> rowdist(0, rows), coldist(0, cols);
		size_t startRow = rowdist(gen), startCol = coldist(gen);
		MyMatrix2D maze;

		generateFullMaze(maze, rows, cols);
		maze[startRow][startCol] = 'O';

		return maze;
	}
}