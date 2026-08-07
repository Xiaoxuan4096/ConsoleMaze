// Layout.cpp - Language and Layout Support.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <vector>
#include <string>
#include <algorithm>

#include "Layout.h"

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
		reader.unlinkFile();

		return defaultTheme;
	}


	void readLayout(std::string currentLanguage, std::string currentTheme, MyLayout& layout, MyFile& reader) {

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
		currentTheme.erase(currentLanguage.end() - 1); // Remove '\n'.
		reader.unlinkFile();

		return std::find(supportThemeList.begin(), supportThemeList.end(), currentTheme) != supportThemeList.end() ? currentTheme : defaultTheme;
	}

	void selectLanguage(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {

	}
	void selectTheme(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {

	}
}