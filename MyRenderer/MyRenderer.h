// MyRenderer.h - Output.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <iostream>

#include "MyMatrix2D.h"

namespace Xiaoxuan4096 {
	class MyRenderer {
	private:
		MyMatrix2D renderBuffer;
	public:
		// Init.
		void clear();

		// Get Buffer.
		void receiveBuffer(MyMatrix2D buffer);
		
		// Output Buffer.
		void output(std::ostream& out = std::cout);
	};
}