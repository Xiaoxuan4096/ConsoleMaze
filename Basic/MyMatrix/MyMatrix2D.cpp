// MyMatrix2D.cpp - Two-dimension Matrix using std::vector as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyMatrix2D.h"

namespace Xiaoxuan4096 {
	namespace Basic {
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
			if (pos >= data.size())
				return;
			data.insert(std::next(data.begin(), pos), str);
			return;
		}
		void MyMatrix2D::deleteRow(size_t pos) {
			if (pos >= data.size())
				return;
			data.erase(std::next(data.begin(), pos));
			return;
		}

		// Get Data.
		std::string& MyMatrix2D::operator[](size_t row) {
			return data[row];
		}
	}
}