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
	void readTranslation1(std::string currentLanguage, MyTranslator& translator, MyFile& reader);
	std::string readCurrentLanguage1(MyFile& reader);

	void selectLanguage1(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW);
}