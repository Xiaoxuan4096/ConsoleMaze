// FileEditor.cpp - Simple File Editor using std::string as underlying.
// Written by Xiaoxuan4096. 

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "FileEditor.h"

namespace Xiaoxuan4096 {
	namespace Basic {
		// Link & Unlink.
		void FileEditor::linkToFile(std::string& path) {
			filePath = path;
			return;
		}
		void FileEditor::linkToFile(std::filesystem::path path) noexcept {
			filePath = path;
			return;
		}
	}
}