// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <conio.h>

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyMatrix2D.h"
#include "MyRenderer.h"
#include "MyTranslator.h"

namespace Xiaoxuan4096 {
    DrawRequestData generateDrawRequestDataFromString(std::string str, size_t startRow, size_t startCol, int defaultDepth = 0) { // Supports \n for a new line.
        DrawRequestData result;
        result.startRow = startRow;
        result.startCol = startCol;
        if (str.empty())
            return result;

        std::string tmpLine = "";
        for (char x : str) {
            if (x == '\n') {
                result.content.addRow(tmpLine, defaultDepth); // Create a new line.
                tmpLine = "";
                continue;
            }
            tmpLine += x;
        }
        result.content.addRow(tmpLine, defaultDepth); // Add the last line and avoid content missing.

        return result;
    }
    MyMatrix2D generateMyMatrix2DFromString(std::string str) { // Supports \n for a new line.
        MyMatrix2D result;
        if (str.empty())
            return result;

        std::string tmpLine = "";
        for (char x : str) {
            if (x == '\n') {
                result.addRow(tmpLine); // Create a new line.
                tmpLine = "";
                continue;
            }
            tmpLine += x;
        }

        return result;
    }

    std::string readCurrentLanguage(MyFile& reader) {
        std::string currentLanguage, supportLanguage;
        std::vector<std::string> supportLanguageList;

        reader.linkToFile("../Configs/CurrentLanguage.dat"); // Need to be modified in release.
        currentLanguage = reader.read();
        currentLanguage.erase(currentLanguage.end() - 1); // Remove '\n'.
        reader.unlinkFile();

        reader.linkToFile("../Translations/SupportLanguageList.dat"); // Need to be modified in release.
        supportLanguage = reader.read();
        reader.unlinkFile();

        std::string tmpLanguage = "";
        for (char x : supportLanguage) {
            if (x == '\n') {
                supportLanguageList.push_back(tmpLanguage);
                tmpLanguage.clear();
                continue;
            }
            tmpLanguage += x;
        }

        return std::find(supportLanguageList.begin(), supportLanguageList.end(), currentLanguage) != supportLanguageList.end() ? currentLanguage : "zh-cn";
    }
    void readTranslation(const std::string& currentLanguage, MyTranslator& translator, MyFile& reader) {
        reader.linkToFile("../Translations/" + currentLanguage + ".lang");
        translator.setTranslationFromFile(reader.read());
        reader.unlinkFile();
        return;
    }
    bool readInt(int& number, int minimal, int maximal, std::istream& in = std::cin) {
        int tmp;
        in >> tmp;
        if (tmp >= minimal && tmp <= maximal) {
            number = tmp;
            return true;
        }
        return false;
    }

    int mainMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
        int mode;

        buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
        renderer.receiveBuffer(buffer.sendBuffer());
        renderer.output();

        while (!readInt(mode, 0, 3)) {
            buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Retry", 0, 3), 5, 0));
            renderer.receiveBuffer(buffer.sendBuffer());
            renderer.output();
            buffer.clear();
            buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
            renderer.receiveBuffer(buffer.sendBuffer());
            renderer.output();
        }

        return mode;
    }

    void game(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
        return;
    }

    void editMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
        return;
    }

    void selectLanguage(MyBuffer& buffer, MyRenderer& renderer, MyFile& reader) {
        return;
    }

    void exitGame(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
        return;
    }

    void mainLogic() {
        // Generic Definitions.
        MyTranslator translator;
        MyFile genericFileRW;
        MyRenderer genericRenderer;
        MyBuffer genericBuffer;
        bool exit = false;

        // Init.
        std::string currentLanguage = readCurrentLanguage(genericFileRW);
        readTranslation(currentLanguage, translator, genericFileRW);
        
        // Game Logic.
        while (!exit)
            switch (mainMenu(translator, genericBuffer, genericRenderer)) {
                case 0:
                    game(translator, genericBuffer, genericRenderer);
                    break;
                case 1:
                    editMenu(translator, genericBuffer, genericRenderer, genericFileRW);
                    break;
                case 2:
                    selectLanguage(genericBuffer, genericRenderer, genericFileRW);
                    break;
                case 3:
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