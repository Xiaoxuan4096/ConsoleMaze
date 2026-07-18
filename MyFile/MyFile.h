// MyFile.h - File reading and writing.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#pragma once

#include <fstream>
#include <string>
#include <filesystem>

namespace Xiaoxuan4096 {
	class MyFile {
	private:
		std::fstream fileProcess;
		std::filesystem::path filePath;
		std::string content;
		bool newestContent = false;
	public:
		// Link And Unlink File.
		void linkToFile(std::filesystem::path path);
		void unlinkFile();

		// Write To File.
		void append(std::string content);
		void rewrite(std::string content);
		void deleteFile(); // Unlink file automatically.

		// Read From File.
		std::string read();

		// Check Status.
		bool exist();
	};
}