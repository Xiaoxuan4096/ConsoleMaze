// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <iostream>
#include <string>

#include <Windows.h>

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
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryInt", 1, 4), 6, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
				buffer.clear();
				buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
				renderer.receiveBuffer(buffer.sendBuffer());
				renderer.output();
			}
		}

		return mode;
	}

	static void exitGame(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, bool exp) {
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
		MyLayout layout;
		bool exit = false, exp = false;

		if (readCurrentLanguage1(genericFileRW) == "zh-cn") {
			std::string confirm;
			std::cout << "本程序正在迁移到一套全新的布局系统。" << std::endl
				<< "\x1b[33m启用实验性布局系统可能使程序出现错误。\x1b[37m" << std::endl
				<< "如果您不希望启用实验性布局系统，请直接按下回车。" << std::endl
				<< "如果您执意启用实验性布局系统，请仔细阅读并输入以下内容，注意不需要输入双引号：" << std::endl
				<< "\"我理解实验性布局系统存在严重稳定性风险。我自愿启用，并在此明确放弃就使用该系统所引发的任何损失，向开发者主张赔偿或追索的权利。\"" << std::endl
				<< ">>> ";
			getline(std::cin, confirm);
			exp = confirm == "我理解实验性布局系统存在严重稳定性风险。我自愿启用，并在此明确放弃就使用该系统所引发的任何损失，向开发者主张赔偿或追索的权利。";
			if (exp)
				std::cout << "已经启用实验性布局系统。" << std::endl;
			else
				std::cout << "将使用传统布局系统。" << std::endl;
			Sleep(3000);
		}

		// Init.
		if (exp)
			readLayout(readCurrentLanguage(genericFileRW), readCurrentTheme(readCurrentLanguage(genericFileRW), genericFileRW), layout, genericFileRW);
		readTranslation1(readCurrentLanguage1(genericFileRW), translator, genericFileRW);

		// Game Logic.
		while (!exit)
			switch (mainMenu(layout, translator, genericBuffer, genericRenderer, exp)) {
				case 1:
					while (game(translator, genericBuffer, genericRenderer, genericFileRW, exp));
					break;
				case 2:
					while (editMenu(translator, genericBuffer, genericRenderer, genericFileRW, exp));
					break;
				case 3:
					selectLanguage1(translator, genericBuffer, genericRenderer, genericFileRW);
					break;
				case 4:
					exitGame(translator, genericBuffer, genericRenderer, exp);
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