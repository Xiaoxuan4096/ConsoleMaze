// MyBuffer.cpp - Collect objects' data and generate a buffer for rendering.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyBuffer.h"

namespace Xiaoxuan4096 {
	void MyBuffer::fetchDrawRequest(DrawRequestData object) {
		
	}
	MyMatrix2D MyBuffer::sendBuffer() {
		MyMatrix2D send;
		for (size_t i = 0; i < buffer.getRowCount(); i++)
			send.addRow(buffer.getLineWithoutDepth(i));
		return send;
	}
}