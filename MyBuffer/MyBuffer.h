// MyBuffer.h - Collect objects' data and generate a buffer for rendering.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include "MyMatrix2D.h"

namespace Xiaoxuan4096 {
	class MyBuffer {
	private:
		MyMatrix2D buffer;
	public:
		template<typename T> void fetchDrawRequest(); // Fetch the draw requests from objects.

		MyMatrix2D sendBuffer(); // Send the buffer to the renderer.
	};
}