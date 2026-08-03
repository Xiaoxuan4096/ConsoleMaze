// GenerateDrawRequestData.cpp - Tools for generating DrawRequestData.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>

#include "MyBuffer.h"
#include "MyMatrix2D.h"

#include "GenerateDrawRequestData.h"

namespace Xiaoxuan4096 {
	DrawRequestData generateDrawRequestDataFromString(std::string str, size_t startRow, size_t startCol, int defaultDepth) { // Supports \n for a new line.
		DrawRequestData result;
		result.startRow = startRow;
		result.startCol = startCol;
		if (str.empty())
			return result;

		std::string tmpLine = "";
		for (char x : str) {
			if (x == '\n') {
				result.content.addRow(tmpLine, defaultDepth); // Create a new line.
				tmpLine = "";
				continue;
			}
			tmpLine += x;
		}
		if (str[str.size() - 1] != '\n')
			result.content.addRow(tmpLine, defaultDepth); // Add the last line and avoid content missing.

		return result;
	}
	DrawRequestData generateDrawRequestDataFromMyMatrix2D(MyMatrix2D matrix, size_t startRow, size_t startCol, int defaultDepth) {
		DrawRequestData result;
		result.startRow = startRow;
		result.startCol = startCol;
		if (matrix.getRowCount() == 0)
			return result;

		for (size_t i = 0; i < matrix.getRowCount(); i++)
			result.content.addRow(matrix[i], defaultDepth);

		return result;
	}
	MyMatrix2D generateMyMatrix2DFromString(std::string str) { // Supports \n for a new line.
		MyMatrix2D result;
		if (str.empty())
			return result;

		std::string tmpLine = "";
		for (char x : str) {
			if (x == '\n') {
				result.addRow(tmpLine); // Create a new line.
				tmpLine = "";
				continue;
			}
			tmpLine += x;
		}

		return result;
	}
}