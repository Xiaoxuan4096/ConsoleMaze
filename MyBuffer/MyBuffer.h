// MyBuffer.h - Collect objects' data and generate a buffer for rendering.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <concepts>
#include "MyMatrix2D.h"
#include "MyMatrix2DWithDepth.h"

namespace Xiaoxuan4096 {
	struct DrawRequestData {
		MyMatrix2DWithDepth content;
		size_t startRow = 0, startCol = 0;
	};

	class MyBuffer {
	private:
		MyMatrix2DWithDepth buffer;
	public:
		template<std::convertible_to<DrawRequestData>... Args> void fetchDrawRequest(Args... objects); // Fetch the draw requests from objects.
		void fetchDrawRequest(DrawRequestData object); // Fetch the draw requests from one object.
		MyMatrix2D sendBuffer(); // Send the buffer to the renderer.
	};

	template<std::convertible_to<DrawRequestData> ...Args> inline void MyBuffer::fetchDrawRequest(Args... objects) {
		(fetchDrawRequest(objects), ...);
		return;
	}
}