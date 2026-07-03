// MyMatrix2DWithDepth.cpp - Two-dimension char Matrix using std::vector as underlying, with depth attached.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyMatrix2DWithDepth.h"

namespace Xiaoxuan4096 {
	// Init Functions.
	void MyMatrix2DWithDepth::clear() {
		data.clear();
		return;
	}

	// Edit Rows.
	void MyMatrix2DWithDepth::addRow(std::string str, int defaultDepth) {
		addRowWithDepth(LineWithDepth(str, defaultDepth));
		return;
	}
	void MyMatrix2DWithDepth::insertRow(std::string str, int defaultDepth, size_t pos) {
		insertRowWithDepth(LineWithDepth(str, defaultDepth), pos);
		return;
	}
	void MyMatrix2DWithDepth::setRow(std::string str, int defaultDepth, size_t pos) {
		setRowWithDepth(LineWithDepth(str, defaultDepth), pos);
		return;
	}

	void MyMatrix2DWithDepth::addRowWithDepth(LineWithDepth line) {
		data.push_back(line);
		return;
	}
	void MyMatrix2DWithDepth::addRowWithDepth(std::string str, std::vector<int> depths) {
		addRowWithDepth(LineWithDepth(str, depths));
		return;
	}
	void MyMatrix2DWithDepth::insertRowWithDepth(LineWithDepth line, size_t pos) {
		if (pos < data.size())
			data.insert(std::next(data.begin(), pos), line);
		return;
	}
	void MyMatrix2DWithDepth::insertRowWithDepth(std::string str, std::vector<int> depths, size_t pos) {
		insertRowWithDepth(LineWithDepth(str, depths), pos);
		return;
	}
	void MyMatrix2DWithDepth::setRowWithDepth(LineWithDepth line, size_t pos) {
		if (pos < data.size())
			data[pos] = line;
		return;
	}
	void MyMatrix2DWithDepth::setRowWithDepth(std::string str, std::vector<int> depths, size_t pos) {
		setRowWithDepth(LineWithDepth(str, depths), pos);
		return;
	}

	void MyMatrix2DWithDepth::deleteRow(size_t pos) {
		if (pos < data.size())
			data.erase(std::next(data.begin(), pos));
		return;
	}

	// Edit Single Cell.
	void MyMatrix2DWithDepth::setCharOnly(char c, size_t row, size_t col) {
		if (row < data.size() && col < data[row].line.size())
			data[row].line[col] = c;
		return;
	}
	void MyMatrix2DWithDepth::setDepthOnly(int depth, size_t row, size_t col) {
		if (row < data.size() && col < data[row].depths.size())
			data[row].depths[col] = depth;
		return;
	}
	void MyMatrix2DWithDepth::setCharAndDepth(char c, int depth, size_t row, size_t col) {
		if (row < data.size() && col < data[row].depths.size()) {
			data[row].line[col] = c;
			data[row].depths[col] = depth;
		}
		return;
	}

	// Get Data.
	std::string MyMatrix2DWithDepth::getLineWithoutDepth(size_t row) {
		return row < data.size() ? data[row].line : "";
	}
	LineWithDepth MyMatrix2DWithDepth::getLineWithDepth(size_t row) {
		return row < data.size() ? data[row] : LineWithDepth();
	}
	size_t MyMatrix2DWithDepth::getRowCount() {
		return data.size();
	}
	size_t MyMatrix2DWithDepth::getMinColCount() {
		if (data.empty())
			return 0;

		size_t minColCount = data[0].line.size();
		for (size_t i = 1; i < data.size(); i++) // If data.size() == 1, this for loop will not be executed.
			minColCount = minColCount > data[i].line.size() ? data[i].line.size() : minColCount;

		return minColCount;
	}
	size_t MyMatrix2DWithDepth::getMaxColCount() {
		if (data.empty())
			return 0;

		size_t maxColCount = data[0].line.size();
		for (size_t i = 1; i < data.size(); i++) // If data.size() == 1, this for loop will not be executed.
			maxColCount = maxColCount < data[i].line.size() ? data[i].line.size() : maxColCount;

		return maxColCount;
	}
}