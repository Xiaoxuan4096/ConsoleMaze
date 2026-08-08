// Layout.cpp - Language and Layout Support.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include <cctype>

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyLayout.h"
#include "MyRenderer.h"

#include "Layout.h"

#include "../DataRW/DataRW.h"

namespace Xiaoxuan4096 {
	static std::vector<std::string> readSupportLanguageList(MyFile& reader) {
		std::string supportLanguage;
		std::vector<std::string> supportLanguageList;

		reader.linkToFile("../Layouts/SupportLanguageList.dat");
		supportLanguage = reader.read();
		reader.unlinkFile();

		std::string tmpLanguage;
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
	static std::string readDefaultLanguage(MyFile& reader) {
		std::string defaultLanguage;

		reader.linkToFile("../Layouts/DefaultLanguage.dat");
		defaultLanguage = reader.read();
		defaultLanguage.erase(defaultLanguage.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return defaultLanguage;
	}
	static std::vector<std::string> readSupportThemeList(std::string currentLanguage, MyFile& reader) {
		std::string supportTheme;
		std::vector<std::string> supportThemeList;

		reader.linkToFile("../Layouts/" + currentLanguage + "/ThemeLists.dat");
		supportTheme = reader.read();
		reader.unlinkFile();

		std::string tmpTheme;
		for (char x : supportTheme) {
			if (x == '\n') {
				supportThemeList.push_back(tmpTheme);
				tmpTheme.clear();
				continue;
			}
			tmpTheme += x;
		}

		return supportThemeList;
	}
	static std::string readDefaultTheme(std::string currentLanguage, MyFile& reader) {
		std::string defaultTheme;

		reader.linkToFile("../Layouts/" + currentLanguage + "/DefaultTheme.dat");
		defaultTheme = reader.read();
		defaultTheme.erase(defaultTheme.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return defaultTheme;
	}
	static void saveCurrentLanguage(std::string currentLanguage, MyFile& writer) {
		writer.linkToFile("../Configs/CurrentLanguage.dat");
		writer.rewrite(currentLanguage);
		writer.unlinkFile();
		return;
	}
	static void saveCurrentTheme(std::string currentTheme, MyFile& writer) {
		writer.linkToFile("../Configs/CurrentTheme.dat");
		writer.rewrite(currentTheme);
		writer.unlinkFile();
		return;
	}

	static DrawRequestData generateDrawRequestDataFromString(std::string str, size_t startRow, size_t startCol, int defaultDepth = 0) { // Supports \n for a new line.
		DrawRequestData result;
		result.startRow = startRow;
		result.startCol = startCol;
		if (str.empty())
			return result;

		std::string tmpLine = "";
		for (char x : str) {
			if (x == '\n') {
				result.content.addRow(tmpLine, defaultDepth); // Create a new line.
				tmpLine = "";
				continue;
			}
			tmpLine += x;
		}
		if (str[str.size() - 1] != '\n')
			result.content.addRow(tmpLine, defaultDepth); // Add the last line and avoid content missing.

		return result;
	}

	static void selectLanguage(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		std::string hint, tmp, inputLanguageToLower = "";
		std::vector<std::string> supportLanguageList = readSupportLanguageList(fileRW);

		fileRW.linkToFile("../Layouts/LanguageHints.dat");
		hint = fileRW.read();
		fileRW.unlinkFile();

		if (!hint.empty() && hint[hint.size() - 1] == '\n')
			hint.erase(hint.end() - 1);

		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), generateDrawRequestDataFromString(hint, 2, 0)); // Magic Number!!!
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		std::getline(std::cin, tmp);
		for (char x : tmp)
			inputLanguageToLower += isalpha(x) ? tolower(x) : x;

		while (std::find(supportLanguageList.begin(), supportLanguageList.end(), inputLanguageToLower) == supportLanguageList.end()) {
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("LanguageNotAvailable"));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), generateDrawRequestDataFromString(hint, 2, 0)); // Magic Number!!!
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			inputLanguageToLower = "";
			std::getline(std::cin, tmp);
			for (char x : tmp)
				inputLanguageToLower += isalpha(x) ? tolower(x) : x;
		}

		saveCurrentLanguage(inputLanguageToLower, fileRW);
		readLayout(inputLanguageToLower, readCurrentTheme(inputLanguageToLower, fileRW), layout, fileRW);
		return;
	}
	static void selectTheme(std::string currentLanguage, MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		std::string hint, tmp, inputTheme = "";
		std::vector<std::string> supportThemeList = readSupportThemeList(currentLanguage, fileRW);

		fileRW.linkToFile("../Layouts/" + currentLanguage + "/ThemeHints.dat");
		hint = fileRW.read();
		fileRW.unlinkFile();

		if (!hint.empty() && hint[hint.size() - 1] == '\n')
			hint.erase(hint.end() - 1);

		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), generateDrawRequestDataFromString(hint, 2, 0)); // Magic Number!!!
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		std::getline(std::cin, inputTheme);
		while (std::find(supportThemeList.begin(), supportThemeList.end(), inputTheme) == supportThemeList.end()) {
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("ThemeNotAvailable"));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), generateDrawRequestDataFromString(hint, 2, 0)); // Magic Number!!!
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			std::getline(std::cin, inputTheme);
		}

		saveCurrentTheme(inputTheme, fileRW);
		readLayout(readCurrentLanguage(fileRW), inputTheme, layout, fileRW);
		return;
	}

	void readLayout(std::string currentLanguage, std::string currentTheme, MyLayout& layout, MyFile& reader) {
		reader.linkToFile("../Layouts/" + currentLanguage + "/" + currentTheme + ".layout");
		layout.setLayoutFromFile(reader.read());
		reader.unlinkFile();
		return;
	}
	std::string readCurrentLanguage(MyFile& reader) {
		std::string currentLanguage, defaultLanguage = readDefaultLanguage(reader);
		std::vector<std::string> supportLanguageList = readSupportLanguageList(reader);

		reader.linkToFile("../Configs/CurrentLanguage.dat");
		currentLanguage = reader.read();
		currentLanguage.erase(currentLanguage.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return std::find(supportLanguageList.begin(), supportLanguageList.end(), currentLanguage) != supportLanguageList.end() ? currentLanguage : defaultLanguage;
	}
	std::string readCurrentTheme(std::string currentLanguage, MyFile& reader) {
		std::string currentTheme, defaultTheme = readDefaultTheme(currentLanguage, reader);
		std::vector<std::string> supportThemeList = readSupportThemeList(currentLanguage, reader);

		reader.linkToFile("../Configs/CurrentTheme.dat");
		currentTheme = reader.read();
		currentTheme.erase(currentTheme.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return std::find(supportThemeList.begin(), supportThemeList.end(), currentTheme) != supportThemeList.end() ? currentTheme : defaultTheme;
	}

	bool selectLanguageAndTheme(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		int mode;
		
		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("LanguageOrThemeMenu"));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		std::string command = readIntInputWithExit(mode, 1, 2);
		while (command == "Fail") {
			buffer.fetchDrawRequest(layout.getLayout("RetryMenuForLanguageOrTheme", 1, 2));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("RetryMenuForLanguageOrTheme"));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			command = readIntInputWithExit(mode, 1, 2);
		}
		if (command == "Exit")
			return false;

		if (mode == 1)
			selectLanguage(layout, buffer, renderer, fileRW);
		else
			selectTheme(readCurrentLanguage(fileRW), layout, buffer, renderer, fileRW);

		return true;
	}
}