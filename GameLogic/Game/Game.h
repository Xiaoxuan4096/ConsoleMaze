// Game.h - Game Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"
#include "MyLayout.h"

namespace Xiaoxuan4096 {
	bool game(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW);
}