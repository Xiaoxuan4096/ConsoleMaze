// Layout.h - Language and Layout Support.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <string>

#include "MyLayout.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"

namespace Xiaoxuan4096 {
	void readLayout(std::string currentLanguage, std::string currentTheme, MyLayout& layout, MyFile& reader);
	std::string readCurrentLanguage(MyFile& reader);
	std::string readCurrentTheme(std::string currentLanguage, MyFile& reader);

	bool selectLanguageAndTheme(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW);
}