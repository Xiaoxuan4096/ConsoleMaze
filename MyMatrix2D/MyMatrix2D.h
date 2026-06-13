// MyMatrix2D.h - Two-dimension char Matrix using std::vector as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <vector>
#include <string>

namespace Xiaoxuan4096 {
	class MyMatrix2D {
	private:
		std::vector<std::string> data;
	public:
		// Init Functions.
		void clear();

		// Edit Functions.
		void addRow(std::string str);
		void insertRow(std::string str, size_t pos); // Note that an invalid position will make no change to the matrix.
		void deleteRow(size_t pos); // Note that an invalid position will make no change to the matrix.

		void setChar(char c, size_t row, size_t col); // Note that an invalid position will make no change to the matrix.
		void setRow(std::string str, size_t pos); // Note that an invalid position will make no change to the matrix.

		// Get Data.
		std::string& operator[](size_t row); // Get a line. Note that giving an invalid row is undefined.
		size_t getRowCount();
		size_t getMinColCount(); // If data is empty, this will return a 0.
		size_t getMaxColCount(); // If data is empty, this will return a 0.
		size_t getColCount(size_t row); // If data is empty or the position is invalid, this will return a 0.
	};
}