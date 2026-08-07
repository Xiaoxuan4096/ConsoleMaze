// MyLayout.h - Translate and Layout Service.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <map>
#include <string>

#include "MyBuffer.h"

namespace Xiaoxuan4096 {
	class MyLayout {
	private:
		std::map<std::string, DrawRequestData> dictionary;
	public:
		void clear();
		void setLayoutFromFile(std::string fileContent); // Use: key = {startRow}, {startCol}, {depth}, "value". Support \n for a new line. Clear current dictionary automatically.
	};
}