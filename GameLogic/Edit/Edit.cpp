// Edit.cpp - Logic of editing levels.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <sstream>
#include <string>
#include <system_error>
#include <filesystem>

#include <Windows.h>

#include "MyTranslator.h"
#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"

#include "Edit.h"

#include "../DataRW/DataRW.h"
#include "../GenerateDrawRequestData/GenerateDrawRequestData.h"

namespace Xiaoxuan4096 {
	static bool openNotepad(std::wstring levelStringW) {
		std::wstring path = L"../Levels/" + levelStringW + L"/Maze.txt";

		SHELLEXECUTEINFOW sei = {};
		sei.cbSize = sizeof(sei);
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = L"open";
		sei.lpFile = L"notepad.exe";
		sei.lpParameters = path.c_str();
		sei.nShow = SW_SHOWNORMAL;

		if (!ShellExecuteExW(&sei))
			return false;
		if (sei.hProcess != NULL && sei.hProcess != INVALID_HANDLE_VALUE) {
			WaitForSingleObject(sei.hProcess, INFINITE);

			DWORD exitCode;
			GetExitCodeProcess(sei.hProcess, &exitCode);
			CloseHandle(sei.hProcess);
		}
		return true;
	}

	static bool createLevel(int level, int maximumLevel, std::string levelString, std::wstring levelStringW, MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		if (level == maximumLevel + 1) {
			std::error_code ec;
			std::filesystem::create_directory("../Levels/" + levelString, ec);
			fileRW.rewrite("");
			fileRW.unlinkFile();

			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("CreateAcception", level), 2, 0), generateDrawRequestDataFromString(translator.getTranslation("EditHint", level), 3, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			if (!openNotepad(levelStringW))
				return false;

			saveMaximumLevel(++maximumLevel, fileRW);
		}
		else {
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("CreateRejection"), 6, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
		}

		return true;
	}
	static bool editLevel(int level, std::string levelString, std::wstring levelStringW, MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("EditHint", level), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		fileRW.unlinkFile();
		fileRW.linkToFile("../Levels/" + levelString + "/Record.dat");
		if (fileRW.exist())
			fileRW.deleteFile();
		else
			fileRW.unlinkFile();

		return openNotepad(levelStringW);
	}
	static void deleteLevel(int level, int maximumLevel, std::string levelString, MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		if (level == maximumLevel) {
			fileRW.deleteFile();
			fileRW.linkToFile("../Levels/" + levelString + "/Record.dat");
			if (fileRW.exist())
				fileRW.deleteFile();
			else
				fileRW.unlinkFile();
			std::error_code ec;
			std::filesystem::remove("../Levels/" + levelString, ec);
		}
		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(level == maximumLevel ? translator.getTranslation("DeleteAcception", level) : translator.getTranslation("DeleteRejection"), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		saveMaximumLevel(level == maximumLevel ? --maximumLevel : maximumLevel, fileRW);
	}

	bool editMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		int level, maximumLevel = readMaximumLevel(fileRW);
		std::stringstream ss;
		std::wstringstream ssW;
		std::string levelString, command;
		std::wstring levelStringW;

		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("EditMenu", maximumLevel, 1, maximumLevel + 1), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		command = readIntInputWithExit(level, 1, maximumLevel + 1);
		while (command == "Fail") {
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryMenu", 1, maximumLevel + 1), 4, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("EditMenu", maximumLevel, 1, maximumLevel + 1), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			command = readIntInputWithExit(level, 1, maximumLevel + 1);
		}
		if (command == "Exit")
			return false;

		ss << level;
		ss >> levelString;
		ssW << level;
		ssW >> levelStringW;

		fileRW.linkToFile("../Levels/" + levelString + "/Maze.txt");
		if (fileRW.exist()) {
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("EditOrDeleteHint", level), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			command = readStringInput(true, "Edit");
			while (command != "Exit" && command != "Delete" && command != "Edit") {
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryEditOrDeleteHint"), 6, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				buffer.clear();
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("EditOrDeleteHint", level), 2, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				command = readStringInput(true);
			}
			if (command == "Delete")
				deleteLevel(level, maximumLevel, levelString, translator, buffer, renderer, fileRW);
			else
				if (command == "Edit")
					if (!editLevel(level, levelString, levelStringW, translator, buffer, renderer, fileRW))
						return false;
		}
		else {
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("CreateHint", level), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			command = readStringInput(true, "Create");
			while (command != "Exit" && command != "Create") {
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryCreateHint"), 6, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				buffer.clear();
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("CreateHint", level), 2, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				command = readStringInput(true, "Create");
			}
			if (command == "Create")
				if (!createLevel(level, maximumLevel, levelString, levelStringW, translator, buffer, renderer, fileRW))
					return false;
		}

		return true;
	}
}