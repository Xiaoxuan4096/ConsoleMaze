// FileEditor.h - Simple File Editor using std::string as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace Xiaoxuan4096 {
	namespace Basic {
		class FileEditor {
		private:
			std::string fileContent; // Large files may cause OOMs; need improvement in the future.
			std::fstream fs;
			std::filesystem::path filePath;
		public:
			// Link & Unlink.
			void linkToFile(std::string& path);
			void linkToFile(std::filesystem::path path) noexcept;
		};
	}
}
