// UnifiedLevelSelector.cpp - A unified level selector for gaming and editing.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>

#include "MyBuffer.h"
#include "MyRenderer.h"

#include "UnifiedLevelSelector.h"

#include "../DataRW/DataRW.h"

namespace Xiaoxuan4096 {
	// TODO: Migrate level selecting logic to here.
	bool unifiedLevelSelector(MyBuffer& commonPrompt, MyBuffer& outOfRangePrompt, int& level, int minimumLevel, int maximumLevel, MyRenderer& renderer) {
		renderer.receiveBuffer(commonPrompt.sendBuffer());
		renderer.output();

		std::string command = readIntInputWithExit(level, minimumLevel, maximumLevel, true);
		while (command == "Fail") {
			renderer.receiveBuffer(outOfRangePrompt.sendBuffer());
			renderer.output();
			renderer.receiveBuffer(commonPrompt.sendBuffer());
			renderer.output();
			command = readIntInputWithExit(level, minimumLevel, maximumLevel, true);
		}

		return command != "Exit";
	}
}