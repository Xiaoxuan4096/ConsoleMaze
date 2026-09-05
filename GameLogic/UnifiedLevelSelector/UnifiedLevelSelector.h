// UnifiedLevelSelector.h - A unified level selector for gaming and editing.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include "MyBuffer.h"
#include "MyRenderer.h"

namespace Xiaoxuan4096 {
	bool unifiedLevelSelector(MyBuffer& commonPrompt, MyBuffer& outOfRangePrompt, int& level, int minimumLevel, int maximumLevel, MyRenderer& renderer); // True for progressing and false for exit.
}