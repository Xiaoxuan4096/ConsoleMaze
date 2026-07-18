// MyFile.cpp - File reading and writing.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <filesystem>
#include <system_error>
#include <string>

#include "MyFile.h"

namespace Xiaoxuan4096 {
	void MyFile::linkToFile(std::filesystem::path path) {
		filePath = path;
		content.clear();
		fileProcess.close();
		fileProcess.clear();
		newestContent = false;
		return;
	}
	void MyFile::unlinkFile() {
		newestContent = false;
		fileProcess.close();
		fileProcess.clear();
		content.clear();
		filePath.clear();
	}

	void MyFile::append(std::string content) {
		fileProcess.open(filePath, std::ios::app);
		if (!fileProcess.is_open()) {
			fileProcess.close();
			fileProcess.clear();
			return;
		}

		for (char x : content) {
			fileProcess << x;
			if (!fileProcess.good()) {
				fileProcess.close();
				fileProcess.clear();
				return;
			}
		}

		fileProcess.close();
		newestContent = false;
		return;
	}
	void MyFile::rewrite(std::string content) {
		fileProcess.open(filePath, std::ios::out);
		if (!fileProcess.is_open()) {
			fileProcess.close();
			fileProcess.clear();
			return;
		}

		for (char x : content) {
			fileProcess << x;
			if (!fileProcess.good()) {
				fileProcess.close();
				fileProcess.clear();
				return;
			}
		}

		fileProcess.close();
		newestContent = false;
		return;
	}
	void MyFile::deleteFile() {
		std::error_code ec;
		if (!filePath.empty())
			std::filesystem::remove(filePath, ec);
		unlinkFile();
		return;
	}

	std::string MyFile::read() {
		if (newestContent)
			return content;

		fileProcess.open(filePath, std::ios::in);
		if (!fileProcess.is_open()) {
			fileProcess.close();
			fileProcess.clear();
			return content;
		}

		content.clear();
		std::string tmp;
		while (std::getline(fileProcess, tmp))
			content.append(tmp).push_back('\n') ;

		if (fileProcess.bad()) {
			content.clear();
			fileProcess.close();
			newestContent = false;
			return content;
		}

		fileProcess.close();
		newestContent = true;
		return content;
	}

	bool MyFile::exist() {
		return std::filesystem::exists(filePath);
	}
}