// MyRenderer.cpp - Output.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyRenderer.h"

namespace Xiaoxuan4096 {
	void MyRenderer::clear() {
		renderBuffer.clear();
		return;
	}

	void MyRenderer::receiveBuffer(MyMatrix2D buffer) {
		renderBuffer = buffer;
		return;
	}

	void MyRenderer::output(std::ostream& out) {
		for (size_t i = 0; i < renderBuffer.getRowCount(); i++)
			out << renderBuffer[i] << std::endl;
		return;
	}
}