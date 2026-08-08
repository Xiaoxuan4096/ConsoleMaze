// Edit.h - Logic of editing levels.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include "MyTranslator.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"

namespace Xiaoxuan4096 {
	bool editMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW, bool exp);
}