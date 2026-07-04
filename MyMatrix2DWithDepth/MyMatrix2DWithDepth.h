// MyMatrix2DWithDepth.h - Two-dimension char Matrix using std::vector as underlying, with depth attached.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <vector>
#include <string>

namespace Xiaoxuan4096 {
	struct LineWithDepth {
		std::string line;
		std::vector<int> depths;

		// Construct Object.
		LineWithDepth() : line(""), depths() {};
		LineWithDepth(std::string str) : line(str), depths(str.size(), 0) {}; // Init from a string. Default depth is 0.
		LineWithDepth(std::string str, int defaultDepth) : line(str), depths(str.size(), defaultDepth) {}; // Set line and default depth.
		LineWithDepth(std::string str, std::vector<int> depth) : line(str) { // Manually set depth information.
			depths.resize(line.size(), 0);
			std::copy(depth.begin(), depth.begin() + std::min(depth.size(), line.size()), depths.begin());
			return;
		};
	};

	class MyMatrix2DWithDepth {
	private:
		std::vector<LineWithDepth> data;
	public:
		// Init Functions.
		void clear();

		// Edit Rows.
		void addRow(std::string str, int defaultDepth = 0);
		void insertRow(std::string str, size_t pos, int defaultDepth = 0); // Note that an invalid position will make no change to the matrix.
		void setRow(std::string str, size_t pos, int defaultDepth = 0); // Note that an invalid position will make no change to the matrix.

		void addRowWithDepth(LineWithDepth line);
		void addRowWithDepth(std::string str, std::vector<int> depths);
		void insertRowWithDepth(LineWithDepth line, size_t pos); // Note that an invalid position will make no change to the matrix.
		void insertRowWithDepth(std::string str, std::vector<int> depths, size_t pos); // Note that an invalid position will make no change to the matrix.
		void setRowWithDepth(LineWithDepth line, size_t pos); // Note that an invalid position will make no change to the matrix.
		void setRowWithDepth(std::string str, std::vector<int> depths, size_t pos); // Note that an invalid position will make no change to the matrix.

		void deleteRow(size_t pos); // Note that an invalid position will make no change to the matrix.

		// Edit Single Cell.
		void setCharOnly(char c, size_t row, size_t col); // Note that an invalid position will make no change to the matrix.
		void setDepthOnly(int depth, size_t row, size_t col); // Note that an invalid position will make no change to the matrix.
		void setCharAndDepth(char c, int depth, size_t row, size_t col); // Note that an invalid position will make no change to the matrix.

		// Get Data.
		std::string getLineWithoutDepth(size_t row); // Get a line and discard depth information. Note that giving an invalid row is an undefined behavior.
		LineWithDepth getLineWithDepth(size_t row); // Get a line with depth information. Note that giving an invalid row is an undefined behavior.
		size_t getRowCount();
		size_t getMinColCount(); // If data is empty, this will return a 0.
		size_t getMaxColCount(); // If data is empty, this will return a 0.
		size_t getColCount(size_t row); // If data is empty or the position is invalid, this will return a 0.
	};
}