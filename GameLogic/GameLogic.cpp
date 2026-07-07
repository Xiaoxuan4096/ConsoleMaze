// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#define NOMINMAX // LineWithDepth in "MyMatrix2DWithDepth.h" uses std::copy, which is incompatible with <Windows.h> without this line.

#include <vector>
#include <string>
#include <algorithm>

#include <Windows.h>

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

        return std::find(supportLanguageList.begin(), supportLanguageList.end(), currentLanguage) != supportLanguageList.end() ? currentLanguage : "en-us";
    }
    void readTranslation(const std::string& currentLanguage, MyTranslator& translator, MyFile& reader) {
        reader.linkToFile("../Translations/" + currentLanguage + ".lang");
        translator.setTranslationFromFile(reader.read());
        reader.unlinkFile();
        return;
    }

    void startHint(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
        buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("MainMenu"), 2, 0));
        renderer.receiveBuffer(buffer.sendBuffer());
        renderer.output();
        return;
    }

    void mainLogic() {
        // Definitions.
        MyTranslator translator;
        MyMatrix2D maze;
        MyFile genericFileRW;
        MyRenderer genericRenderer;
        MyBuffer genericBuffer;

        // Init.
        std::string currentLanguage = readCurrentLanguage(genericFileRW);
        readTranslation(currentLanguage, translator, genericFileRW);
        startHint(translator, genericBuffer, genericRenderer);

        return;
    }
}

int main() {
    Xiaoxuan4096::mainLogic();
    return 0;
}