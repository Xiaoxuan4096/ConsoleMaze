// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <string>

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyRenderer.h"
#include "MyTranslator.h"
#include "MyLayout.h"

#include "GenerateDrawRequestData/GenerateDrawRequestData.h"
#include "DataRW/DataRW.h"
#include "Game/Game.h"
#include "Edit/Edit.h"
#include "Language/Language.h"
#include "Layout/Layout.h"

namespace Xiaoxuan4096 {
	static int mainMenu(MyLayout& layout, MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, bool exp) {
		int mode;
		
		if (!exp){
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
		}
		else {
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
		}

		return mode;
	}

	static void exitGame(MyLayout& layout, MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, bool exp) {
		if (!exp)
			buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Exit"), 6, 0));
		else
			buffer.fetchDrawRequest(layout.getLayout("Exit"));
		renderer.receiveBuffer(buffer.sendBuffer());
		renderer.output();
		return;
	}

	static void mainLogic(bool exp) {
		// Generic Definitions.
		MyTranslator translator;
		MyFile genericFileRW;
		MyRenderer genericRenderer;
		MyBuffer genericBuffer;
		MyLayout layout;
		bool exit = false;

		// Init.
		if (exp)
			readLayout(readCurrentLanguage(genericFileRW), readCurrentTheme(readCurrentLanguage(genericFileRW), genericFileRW), layout, genericFileRW);
		else
			readTranslation1(readCurrentLanguage1(genericFileRW), translator, genericFileRW);

		// Game Logic.
		while (!exit)
			switch (mainMenu(layout, translator, genericBuffer, genericRenderer, exp)) {
				case 1:
					while (game(layout, translator, genericBuffer, genericRenderer, genericFileRW, exp));
					break;
				case 2:
					while (editMenu(layout, translator, genericBuffer, genericRenderer, genericFileRW, exp));
					break;
				case 3:
					if (!exp)
						selectLanguage1(translator, genericBuffer, genericRenderer, genericFileRW);
					else
						while (selectLanguageAndTheme(layout, genericBuffer, genericRenderer, genericFileRW));
					break;
				case 4:
					exitGame(layout, translator, genericBuffer, genericRenderer, exp);
					exit = true;
					break;
				default:
					break;
			}

		return;
	}
}

int main(int argc, char* argv[]) {
	Xiaoxuan4096::mainLogic(argc != 2 || std::string(argv[1]) != "--Legacy");
	return 0;
}