// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyRenderer.h"
#include "MyLayout.h"

#include "DataRW/DataRW.h"
#include "Game/Game.h"
#include "Edit/Edit.h"
#include "Layout/Layout.h"

namespace Xiaoxuan4096 {
	static int mainMenu(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer) {
		int mode;

		buffer.clear();
		buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("MainMenu"));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();

		while (!readIntInput(mode, 1, 4)) {
			buffer.fetchDrawRequest(layout.getLayout("RetryIntForMainMenu", 1, 4));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
			buffer.clear();
			buffer.fetchDrawRequest(layout.getLayout("Title"), layout.getLayout("MainMenu"));
			renderer.receiveBuffer(buffer.sendBuffer());
			renderer.output();
		}

		return mode;
	}

	static void exitGame(MyLayout& layout, MyBuffer& buffer, MyRenderer& renderer) {
		buffer.fetchDrawRequest(layout.getLayout("Exit"));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();
		return;
	}

	static void mainLogic() {
		// Generic Definitions.
		MyFile genericFileRW;
		MyRenderer genericRenderer;
		MyBuffer genericBuffer;
		MyLayout layout;
		bool exit = false;

		// Init.
		readLayout(readCurrentLanguage(genericFileRW), readCurrentTheme(readCurrentLanguage(genericFileRW), genericFileRW), layout, genericFileRW);

		// Game Logic.
		while (!exit)
			switch (mainMenu(layout, genericBuffer, genericRenderer)) {
				case 1:
					while (game(layout, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 2:
					while (editMenu(layout, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 3:
					while (selectLanguageAndTheme(layout, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 4:
					exitGame(layout, genericBuffer, genericRenderer);
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