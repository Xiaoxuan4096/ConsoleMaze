// MyMatrix2D.cpp - Two-dimension char Matrix using std::vector as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyMatrix2D.h"

namespace Xiaoxuan4096 {
	// Init Functions.
	void MyMatrix2D::clear() {
		data.clear();
		return;
	}

	// Edit Functions.
	void MyMatrix2D::addRow(std::string str) {
		data.push_back(str);
		return;
	}
	void MyMatrix2D::insertRow(std::string str, size_t pos) {
		if (pos < data.size())
			data.insert(std::next(data.begin(), pos), str);
		return;
	}
	void MyMatrix2D::deleteRow(size_t pos) {
		if (pos < data.size())
			data.erase(std::next(data.begin(), pos));
		return;
	}

	void MyMatrix2D::setChar(char c, size_t row, size_t col) {
		if (row < data.size() && col < data[row].size())
			data[row][col] = c;
		return;
	}
	void MyMatrix2D::setRow(std::string str, size_t pos) {
		if (pos < data.size())
			data[pos] = str;
		return;
	}

	// Get Data.
	std::string& MyMatrix2D::operator[](size_t row) {
		return data[row];
	}
	size_t MyMatrix2D::getRowCount() {
		return data.size();
	}
	size_t MyMatrix2D::getMinColCount() {
		if (data.empty())
			return 0;

		size_t minColCount = data[0].size();
		for (size_t i = 1; i < data.size(); i++) // If data.size() == 1, this for loop will not be executed.
			minColCount = minColCount > data[i].size() ? data[i].size() : minColCount;

		return minColCount;
	}
	size_t MyMatrix2D::getMaxColCount() {
		if (data.empty())
			return 0;

		size_t maxColCount = data[0].size();
		for (size_t i = 1; i < data.size(); i++) // If data.size() == 1, this for loop will not be executed.
			maxColCount = maxColCount < data[i].size() ? data[i].size() : maxColCount;

		return maxColCount;
	}
	size_t MyMatrix2D::getColCount(size_t row) {
		return (data.empty() || row >= data.size()) ? 0 : data[row].size();
	}
}