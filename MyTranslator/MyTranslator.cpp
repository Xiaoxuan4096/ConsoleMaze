// MyTranslator.cpp - Translate Service.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>

#include "MyTranslator.h"

namespace Xiaoxuan4096 {
	void MyTranslator::editTranslation(std::string original, std::string translation) {
		dictionary[original] = translation;
		return;
	}
	void MyTranslator::setTranslationFromFile(std::string fileContent) {
		clear();

		for (size_t i = 0; i < fileContent.size(); i++) {
			std::string tmpKey = "", tmpValue = "";
			for (; fileContent[i] != ' '; i++)
				tmpKey += fileContent[i];
			if (fileContent[i] != ' ' || fileContent[i + 1] != '=' || fileContent[i + 2] != ' ') // Check format.
				return;
			for (i += 3; fileContent[i] != '\n'; i++) {
				if (fileContent[i] == '\\') {
					switch (fileContent[++i]) {
						case 'n':
							tmpValue += '\n';
							break;
						case 'r':
							tmpValue += '\r';
							break;
						case 't':
							tmpValue += '\t';
							break;
						case 'v':
							tmpValue += '\v';
							break;
						case 'b':
							tmpValue += '\b';
							break;
						case 'f':
							tmpValue += '\f';
							break;
						case 'a':
							tmpValue += '\a';
							break;
						case '\\':
							tmpValue += '\\';
							break;
						case '\'':
							tmpValue += '\'';
							break;
						case '\"':
							tmpValue += '\"';
							break;
						case '\?':
							tmpValue += '\?';
							break;
						default:
							tmpValue += '\\';
							tmpValue += fileContent[i];
							break;
					}
					continue;
				}
				tmpValue += fileContent[i];
			}
			dictionary[tmpKey] = tmpValue;
		}

		return;
	}

	void MyTranslator::deleteTranslation(std::string original) {
		if (dictionary.contains(original))
			dictionary.erase(original);
		return;
	}
	void MyTranslator::clear() {
		dictionary.clear();
		return;
	}

	std::string MyTranslator::getTranslation(std::string original) {
		return dictionary.contains(original) ? dictionary[original] : "";
	}
}