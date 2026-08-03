// GenerateDrawRequestData.h - Tools for generating DrawRequestData.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <string>

#include "MyMatrix2D.h"
#include "MyBuffer.h"

namespace Xiaoxuan4096 {
	DrawRequestData generateDrawRequestDataFromString(std::string str, size_t startRow, size_t startCol, int defaultDepth = 0); // Supports \n for a new line.
	DrawRequestData generateDrawRequestDataFromMyMatrix2D(MyMatrix2D matrix, size_t startRow, size_t startCol, int defaultDepth = 0);
	MyMatrix2D generateMyMatrix2DFromString(std::string str); // Supports \n for a new line.
}