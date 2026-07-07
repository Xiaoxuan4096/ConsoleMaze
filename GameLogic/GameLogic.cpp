// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#define NOMINMAX // LineWithDepth in "MyMatrix2DWithDepth.h" uses std::copy, which is incompatible with <Windows.h> without this line.

#include <string>

#include <Windows.h>

#include "MyBuffer.h"
#include "MyFile.h"
#include "MyMatrix2D.h"
#include "MyMatrix2DWithDepth.h"
#include "MyRenderer.h"

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


    void startHint() {
        generateDrawRequestDataFromString("ConsoleMase\n", 0, 0);
        return;
    }

    void mainLogic() {
        MyMatrix2D mase;
        MyFile maseRW;

        return;
    }
}

int main() {
    Xiaoxuan4096::mainLogic();
    return 0;
}