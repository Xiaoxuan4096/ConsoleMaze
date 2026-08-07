// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyRenderer.h"
#include "MyLayout.h"

#include "GenerateDrawRequestData/GenerateDrawRequestData.h"
#include "DataRW/DataRW.h"
#include "Game/Game.h"
#include "Edit/Edit.h"
#include "Layout/Layout.h"

namespace Xiaoxuan4096 {
	static int mainMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
		int mode;

		buffer.clear();
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		while (!readIntInput(mode, 1, 4)) {
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryInt", 1, 4), 6, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
		}

		return mode;
	}

	static void exitGame(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
		buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Exit"), 6, 0));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();
		return;
	}

	static void mainLogic() {
		// Generic Definitions.
		MyTranslator translator;
		MyFile genericFileRW;
		MyRenderer genericRenderer;
		MyBuffer genericBuffer;
		bool exit = false;

		// Init.
		readTranslation(readCurrentLanguage(genericFileRW), translator, genericFileRW);

		// Game Logic.
		while (!exit)
			switch (mainMenu(translator, genericBuffer, genericRenderer)) {
				case 1:
					while (game(translator, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 2:
					while (editMenu(translator, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 3:
					selectLanguage(translator, genericBuffer, genericRenderer, genericFileRW);
					break;
				case 4:
					exitGame(translator, genericBuffer, genericRenderer);
					exit = true;
					break;
				default:
					break;
			}

		return;
	}
}

int main() {
	Xiaoxuan4096::mainLogic();
	return 0;
}