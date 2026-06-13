// MyBuffer.cpp - Collect objects' data and generate a buffer for rendering.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyBuffer.h"

namespace Xiaoxuan4096 {
	void MyBuffer::clear() {
		buffer.clear();
		return;
	}

	void MyBuffer::fetchDrawRequest(DrawRequestData object) {
		if (object.startRow >= buffer.getRowCount()) // Create blank rows if necessary.
			for (size_t i = buffer.getRowCount(); i < object.startRow + object.content.getRowCount(); i++)
				buffer.addRow("");

		for (size_t i = object.startRow; i < object.content.getRowCount() + object.startRow; i++) {
			if (object.startCol + object.content.getLineWithoutDepth(i - object.startRow).size() > buffer.getColCount(i)) { // Create blank cols if necessary.
				std::vector<int> tmpDepths = buffer.getLineWithDepth(i).depths; // Save original depths data.
				buffer.setRow(buffer.getLineWithoutDepth(i) + std::string(object.content.getLineWithoutDepth(i - object.startRow).size(), ' '), i);
				for (size_t j = 0; j < tmpDepths.size(); j++) // Recovery original depths data.
					buffer.setDepthOnly(tmpDepths[j], i, j);
			}
			for (size_t j = 0; j < object.content.getLineWithDepth(i - object.startRow).line.size(); j++) // Iterate the line.
				buffer.setCharAndDepth(object.content.getLineWithDepth(i - object.startRow).line[j], object.content.getLineWithDepth(i - object.startRow).depths[j], i, object.startCol + j);
		}

		return;
	}

	MyMatrix2D MyBuffer::sendBuffer() {
		MyMatrix2D send;
		for (size_t i = 0; i < buffer.getRowCount(); i++)
			send.addRow(buffer.getLineWithoutDepth(i));
		return send;
	}
}