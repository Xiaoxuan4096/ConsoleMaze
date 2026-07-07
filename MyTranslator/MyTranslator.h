// MyTranslator.h - Translate Service.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <map>
#include <string>
#include <format>

namespace Xiaoxuan4096 {
	class MyTranslator {
	private:
		std::map<std::string, std::string> dictionary;
	public:
		// Edit Translation.
		void editTranslation(std::string original, std::string translation); // Support add and edit.
		void setTranslationFromFile(std::string fileContent); // Use: key = value. Support \n for a new line. Clear current dictionary automatically.

		// Delete Translation.
		void deleteTranslation(std::string original);
		void clear();

		// Get Translation.
		std::string getTranslation(std::string original);
		template<typename... Args> std::string getTranslation(std::string original, Args&&... args);
	};

	template<typename ...Args> inline std::string MyTranslator::getTranslation(std::string original, Args && ...args) {
		return std::vformat(dictionary.contains(original) ? dictionary[original] : "", std::make_format_args(args...)));
	}
}