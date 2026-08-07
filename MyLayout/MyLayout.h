// MyLayout.h - Translate and Layout Service.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <map>
#include <string>
#include <utility>
#include <format>

#include "MyBuffer.h"

namespace Xiaoxuan4096 {
	struct LayoutData {
		size_t startRow = 0, startCol = 0;
		int depth = 0;
		std::string value;
	};

	class MyLayout {
	private:
		std::map<std::string, LayoutData> dictionary;
	private:
		std::pair<std::string, LayoutData> splitSingleLine(std::string singleLine);
		DrawRequestData LayoutDataToDrawRequestData(LayoutData layout);
	public:
		void clear();
		void setLayoutFromFile(std::string fileContent); // Use: key = {startRow}, {startCol}, {depth}, "value". Support \n for a new line. Clear current dictionary automatically.
		DrawRequestData getLayout(std::string original); // Return original text if no such layout exists.
		template<typename... Args> DrawRequestData getLayout(std::string original, Args&&... args); // Return original text if no such layout exists.
	};

	template<typename ...Args> inline DrawRequestData MyLayout::getLayout(std::string original, Args && ...args) {
		if (!dictionary.contains(original))
			return LayoutDataToDrawRequestData(splitSingleLine(std::string("tmp = 0, 0, 0, ") + original).second);
		std::string newValue =  std::vformat(dictionary[original].value, std::make_format_args(args...));
		LayoutData newLayout = {dictionary[original].startRow, dictionary[original].startCol, dictionary[original].depth, newValue};
		return LayoutDataToDrawRequestData(newLayout);
	}
}