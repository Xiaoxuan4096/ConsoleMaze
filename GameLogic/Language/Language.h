// Language.h - Language Support.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <string>

#include "MyTranslator.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"

namespace Xiaoxuan4096 {
	void readTranslation(std::string currentLanguage, MyTranslator& translator, MyFile& reader);
	std::string readCurrentLanguage(MyFile& reader);

	void selectLanguage(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW);
}