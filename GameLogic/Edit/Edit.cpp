// Edit.cpp - Logic of editing levels.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <sstream>
#include <string>
#include <system_error>
#include <filesystem>

#include <Windows.h>

#include "MyBuffer.h"
#include "MyRenderer.h"
#include "MyFile.h"
#include "MyLayout.h"

#include "Edit.h"

#include "../DataRW/DataRW.h"

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

	static bool createLevel(int level, int maximumLevel, std::string levelString, std::wstring levelStringW, MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		if (level == maximumLevel + 1) {
			std::error_code ec;
			std::filesystem::create_directory("../Levels/" + levelString, ec);
			fileRW.rewrite("");
			fileRW.unlinkFile();

			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("CreateAcception", level), layout.getLayout("EditHintForCreate", level));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			if (!openNotepad(levelStringW))
				return false;

			saveMaximumLevel(++maximumLevel, fileRW);
		}
		else {
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("CreateRejection"));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
		}

		return true;
	}
	static bool editLevel(int level, std::string levelString, std::wstring levelStringW, MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("EditHintForEdit", level));
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
	static void deleteLevel(int level, int maximumLevel, std::string levelString, MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
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
		buffer.fetchDrawRequest(layout.getLayout("Title"), level == maximumLevel ? layout.getLayout("DeleteAcception", level) : layout.getLayout("DeleteRejection"));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		saveMaximumLevel(level == maximumLevel ? --maximumLevel : maximumLevel, fileRW);
	}

	bool editMenu(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
		int level, maximumLevel = readMaximumLevel(fileRW);
		std::stringstream ss;
		std::wstringstream ssW;
		std::string levelString, command;
		std::wstring levelStringW;

		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("EditMenu", maximumLevel, 1, maximumLevel + 1));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		command = readIntInputWithExit(level, 1, maximumLevel + 1);
		while (command == "Fail") {
			buffer.fetchDrawRequest(layout.getLayout("RetryMenuForEditMenu", 1, maximumLevel + 1));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("EditMenu", maximumLevel, 1, maximumLevel + 1));
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
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("EditOrDeleteHint", level));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			command = readStringInput(true, "Edit");
			while (command != "Exit" && command != "Delete" && command != "Edit") {
				buffer.fetchDrawRequest(layout.getLayout("RetryEditOrDeleteHint"));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				buffer.clear();
				buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("EditOrDeleteHint", level));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				command = readStringInput(true);
			}
			if (command == "Delete")
				deleteLevel(level, maximumLevel, levelString, layout, buffer, renderer, fileRW);
			else
				if (command == "Edit")
					if (!editLevel(level, levelString, levelStringW, layout, buffer, renderer, fileRW))
						return false;
		}
		else {
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("CreateHint", level));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();

			command = readStringInput(true, "Create");
			while (command != "Exit" && command != "Create") {
				buffer.fetchDrawRequest(layout.getLayout("RetryCreateHint"));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				buffer.clear();
				buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("CreateHint", level));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				command = readStringInput(true, "Create");
			}
			if (command == "Create")
				if (!createLevel(level, maximumLevel, levelString, levelStringW, layout, buffer, renderer, fileRW))
					return false;
		}

		return true;
	}
}