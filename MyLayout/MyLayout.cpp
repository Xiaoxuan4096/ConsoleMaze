// MyLayout.cpp - Translate and Layout Service.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <vector>
#include <string>
#include <sstream>
#include <utility>

#include <cctype>

#include "MyBuffer.h"

#include "MyLayout.h"

namespace Xiaoxuan4096 {
	static size_t stringToSizeT(std::string str) {
		std::stringstream ss;
		size_t result;
		ss << str;
		ss >> result;
		return result;
	}
	static int stringToInt(std::string str) {
		std::stringstream ss;
		int result;
		ss << str;
		ss >> result;
		return result;
	}
	DrawRequestData MyLayout::LayoutDataToDrawRequestData(LayoutData layout) {
		DrawRequestData result;
		result.startRow = layout.startRow;
		result.startCol = layout.startCol;
		if (layout.value.empty())
			return result;

		std::string tmpLine;
		for (char x : layout.value) {
			if (x == '\n') {
				result.content.addRow(tmpLine, layout.depth); // Create a new line.
				tmpLine.clear();
				continue;
			}
			tmpLine += x;
		}
		if (!layout.value.empty() && layout.value[layout.value.size() - 1] != '\n')
			result.content.addRow(tmpLine, layout.depth); // Add the last line and avoid content missing.

		return result;
	}

	std::pair<std::string, LayoutData> MyLayout::splitSingleLine(std::string singleLine) { // Use: key = {startRow}, {startCol}, {depth}, "value".
		size_t pos;
		std::string tmp, key, value;
		LayoutData result;

		for (pos = 0; pos < singleLine.size() && singleLine[pos] != '='; pos++)
			if (singleLine[pos] != ' ')
				tmp += singleLine[pos];
		key = tmp;

		tmp.clear();
		for (pos++; pos < singleLine.size() && singleLine[pos] != ','; pos++)
			if (isdigit(singleLine[pos]))
				tmp += singleLine[pos];
		result.startRow = stringToSizeT(tmp);

		tmp.clear();
		for (pos++; pos < singleLine.size() && singleLine[pos] != ','; pos++)
			if (isdigit(singleLine[pos]))
				tmp += singleLine[pos];
		result.startCol = stringToSizeT(tmp);

		tmp.clear();
		for (pos++; pos < singleLine.size() && singleLine[pos] != ','; pos++)
			if (isdigit(singleLine[pos]) || singleLine[pos] == '-')
				tmp += singleLine[pos];
		result.depth = stringToInt(tmp);

		tmp.clear();
		for (pos++; pos < singleLine.size() && singleLine[pos] != '\"'; pos++); // Move to the start of value.
		for (pos++; pos < singleLine.size() && !(singleLine[pos] == '\"' && singleLine[pos - 1] != '\\'); pos++) {
			if (singleLine[pos] == '\\' && pos + 1 < singleLine.size()) {
				switch (singleLine[++pos]) {
					case 'n':
						tmp += '\n';
						break;
					case 'r':
						tmp += '\r';
						break;
					case 't':
						tmp += '\t';
						break;
					case 'v':
						tmp += '\v';
						break;
					case 'b':
						tmp += '\b';
						break;
					case 'f':
						tmp += '\f';
						break;
					case 'a':
						tmp += '\a';
						break;
					case '\\':
						tmp += '\\';
						break;
					case '\'':
						tmp += '\'';
						break;
					case '\"':
						tmp += '\"';
						break;
					case '\?':
						tmp += '\?';
						break;
					case '0':
						if (pos + 2 < singleLine.size() && singleLine[pos + 1] == '3' && singleLine[pos + 2] == '3') {
							tmp += '\033';
							pos += 2;
						}
						else
							tmp += '\0';
						break;
					case 'x':
						if (pos + 2 < singleLine.size() && singleLine[pos + 1] == '1' && singleLine[pos + 2] == 'b') {
							tmp += '\x1b';
							pos += 2;
						}
						else {
							tmp += '\\';
							tmp += singleLine[pos];
						}
						break;
					default:
						tmp += '\\';
						tmp += singleLine[pos];
						break;
				}
				continue;
			}
			tmp += singleLine[pos];
		}
		result.value = tmp;

		return std::make_pair(key, result);
	}

	void MyLayout::clear() {
		dictionary.clear();
		return;
	}

	void MyLayout::setLayoutFromFile(std::string fileContent) {
		clear();

		std::vector<std::string> layoutList;
		std::string tmp;
		for (size_t i = 0; i < fileContent.size(); i++) { // Split multiple lines.
			if (!tmp.empty() && fileContent[i] == '\n' && tmp[0] != '#') {
				layoutList.push_back(tmp);
				tmp.clear();
				continue;
			}
			tmp += fileContent[i];
		}

		for (std::string str : layoutList)
			dictionary.insert(splitSingleLine(str));
		return;
	}

	DrawRequestData MyLayout::getLayout(std::string original) {
		if (dictionary.contains(original))
			return LayoutDataToDrawRequestData(dictionary[original]);
		return LayoutDataToDrawRequestData(splitSingleLine("tmp = 0, 0, 0, \"" + original + "\"").second);
	}
}