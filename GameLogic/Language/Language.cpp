// Language.cpp - Language Support.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <cctype>

#include "MyTranslator.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"

#include "Language.h"

#include "../GenerateDrawRequestData/GenerateDrawRequestData.h"

namespace Xiaoxuan4096 {
	static std::vector<std::string> readSupportLanguageList(MyFile& reader) {
		std::string supportLanguage;
		std::vector<std::string> supportLanguageList;

		reader.linkToFile("../Translations/SupportLanguageList.dat");
		supportLanguage = reader.read();
		reader.unlinkFile();

		std::string tmpLanguage = "";
		for (char x : supportLanguage) {
			if (x == '\n') {
				supportLanguageList.push_back(tmpLanguage);
				tmpLanguage.clear();
				continue;
			}
			tmpLanguage += x;
		}

		return supportLanguageList;
	}
	
	static void saveCurrentLanguage(std::string currentLanguage, MyFile& writer) {
		writer.linkToFile("../Configs/CurrentLanguage.dat");
		writer.rewrite(currentLanguage);
		writer.unlinkFile();
		return;
	}

	void readTranslation(std::string currentLanguage, MyTranslator& translator, MyFile& reader) {
		reader.linkToFile("../Translations/" + currentLanguage + ".lang");
		translator.setTranslationFromFile(reader.read());
		reader.unlinkFile();
		return;
	}
	std::string readCurrentLanguage(MyFile& reader) {
		std::string currentLanguage;
		std::vector<std::string> supportLanguageList = readSupportLanguageList(reader);

		reader.linkToFile("../Configs/CurrentLanguage.dat");
		currentLanguage = reader.read();
		currentLanguage.erase(currentLanguage.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return std::find(supportLanguageList.begin(), supportLanguageList.end(), currentLanguage) != supportLanguageList.end() ? currentLanguage : "zh-cn";
	}

	void selectLanguage(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		std::string hint, tmp, inputLanguageToLower = "";
		std::vector<std::string> supportLanguageList = readSupportLanguageList(fileRW);

		fileRW.linkToFile("../Translations/LanguageHints.dat");
		hint = fileRW.read();
		fileRW.unlinkFile();

		if (hint[hint.size() - 1] == '\n')
			hint.erase(hint.end() - 1);

		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(hint, 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		std::getline(std::cin, tmp);
		for (char x : tmp)
			inputLanguageToLower += isalpha(x) ? tolower(x) : x;

		while (std::find(supportLanguageList.begin(), supportLanguageList.end(), inputLanguageToLower) == supportLanguageList.end()) {
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("LanguageNotAvailable"), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(hint, 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			inputLanguageToLower = "";
			std::getline(std::cin, tmp);
			for (char x : tmp)
				inputLanguageToLower += isalpha(x) ? tolower(x) : x;
		}

		saveCurrentLanguage(inputLanguageToLower, fileRW);
		readTranslation(inputLanguageToLower, translator, fileRW);
		return;
	}
}