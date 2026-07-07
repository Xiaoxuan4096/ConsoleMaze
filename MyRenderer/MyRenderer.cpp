// MyRenderer.cpp - Output.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <sstream>

#include <cstdlib>

#include <Windows.h>

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
		system("cls");
		for (size_t i = 0; i < renderBuffer.getRowCount(); i++) {
			for (size_t j = 0; j < renderBuffer.getColCount(i); j++) {
				if (renderBuffer[i][j] == '\\' && renderBuffer[i][j + 1] == 's') { // Support for \sd sleeping. d is a decimal number range from 0 to 9.
					int sleepSecond;
					std::stringstream ss;

					j += 2;
					ss << renderBuffer[i][j++];
					ss >> sleepSecond;

					Sleep(sleepSecond * 1000);
				}
				out << renderBuffer[i][j];
			}
			out << std::endl;
		}
		return;
	}
}