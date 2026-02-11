// MyMatrix2D.h - Two-dimension Matrix using std::vector as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <vector>
#include <string>

namespace Xiaoxuan4096 {
	namespace Basic {
		class MyMatrix2D {
		private:
			std::vector<std::string> data;
		public:
			// Init Functions.
			void clear() ;

			// Edit Functions.
			void addRow(std::string str) ;
			void insertRow(std::string str, size_t pos) ; // Note that an invalid position will make no change to the matrix.
			void deleteRow(size_t pos) ; // Note that an invalid position will make no change to the matrix.

			// Get Data.
			std::string& operator[](size_t row); // Get a line. Note that giving an invalid row is undefined.
		};
	}
}