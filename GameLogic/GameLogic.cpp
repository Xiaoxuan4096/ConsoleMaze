// GameLogic.cpp - Main Logic.
// Written by Xiaoxuan4096.

// This program is distributed under MIT License.
// See LICENSE.txt for details.

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#include <conio.h>

// Debug starts.
#define NOMINMAX
#include <Windows.h>
// Debug ends.

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

        reader.linkToFile("../Configs/CurrentLanguage.dat");
        currentLanguage = reader.read();
        currentLanguage.erase(currentLanguage.end() - 1); // Remove '\n'.
        reader.unlinkFile();

        reader.linkToFile("../Translations/SupportLanguageList.dat");
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
    bool readIntInput(int& number, int minimal, int maximal, bool enterToSkip = false, std::istream& in = std::cin) {
        std::stringstream ss;
        std::string input;
        int tmp;

        std::getline(in, input);
        if (input == "" && enterToSkip)
            return true;

        ss << input;
        ss >> tmp;
        
        if (tmp >= minimal && tmp <= maximal) {
            number = tmp;
            return true;
        }
        return false;
    }

    int mainMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
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

    int readCurrentLevel(MyFile& reader) {
        int currentLevel = 1;
        std::stringstream ss;

        reader.linkToFile("../Levels/CurrentLevel.dat");
        ss << reader.read();
        reader.unlinkFile();

        if (ss.str() != "")
            ss >> currentLevel;
        return currentLevel;
    }
    void saveCurrentLevel(int currentLevel, MyFile& writer) {
        std::stringstream ss;
        std::string tmp;

        ss << currentLevel;
        ss >> tmp;

        writer.linkToFile("../Levels/CurrentLevel.dat");
        writer.rewrite(tmp);
        writer.unlinkFile();

        return;
    }
    int readMaximumLevel(MyFile& reader) {
        int maximumLevel = 1;
        std::stringstream ss;

        reader.linkToFile("../Levels/MaximumLevel.dat");
        ss << reader.read();
        reader.unlinkFile();

        if (ss.str() != "")
            ss >> maximumLevel;
        return maximumLevel;
    }
    void saveMaximumLevel(int maximumLevel, MyFile& writer) {
        std::stringstream ss;
        std::string tmp;

        ss << maximumLevel;
        ss >> tmp;

        writer.linkToFile("../Levels/MaximumLevel.dat");
        writer.rewrite(tmp);
        writer.unlinkFile();

        return;
    }
    
    MyMatrix2D readLevelMaze(int level, MyFile& reader) {
        std::stringstream ss;
        std::string levelString;
        MyMatrix2D maze;

        ss << level;
        ss >> levelString;

        reader.linkToFile("../Levels/" + levelString + "/Maze.txt");
        maze = generateMyMatrix2DFromString(reader.read());
        reader.unlinkFile();

        return maze;
    }
    double readLevelRecord(int level, MyFile& reader) {
        std::stringstream ss;
        std::string levelString, recordString;
        double record;

        ss << level;
        ss >> levelString;

        reader.linkToFile("../Levels/" + levelString + "/Record.dat");
        recordString = reader.read();
        reader.unlinkFile();

        if (recordString == "")
            record = std::numeric_limits<double>::max();
        else {
            ss.clear();
            ss << recordString;
            ss >> record;
        }

        return record;
    }
    void saveLevelRecord(int level, double record, MyFile& writer) {
        std::stringstream ss;
        std::string levelString, recordString;

        ss << level;
        ss >> levelString;
        ss.clear();
        ss << record;
        ss >> recordString;

        writer.linkToFile("../Levels/" + levelString + "/Record.dat");
        writer.rewrite(recordString);
        writer.unlinkFile();

        return;
    }
    std::string readIntInputWithExit(int& number, int minimal, int maximal, bool enterToSkip = false, std::istream& in = std::cin) {
        std::stringstream ss;
        std::string input;
        int tmp;

        std::getline(in, input);
        if (input == "" && enterToSkip)
            return "Progress";
        if (input == "exit")
            return "Exit";

        ss << input;
        ss >> tmp;

        if (tmp >= minimal && tmp <= maximal) {
            number = tmp;
            return "Progress";
        }
        return "Fail";
    }

    void game(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
        int currentLevel = readCurrentLevel(fileRW), maximumLevel = readMaximumLevel(fileRW), level;
        MyMatrix2D maze;
        double record;
        size_t endx, endy, currentx, currenty;
        
        buffer.clear();
        buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("SelectLevel", currentLevel, 1, currentLevel), 2, 0));
        renderer.receiveBuffer(buffer.sendBuffer());
        renderer.output();

        std::string command = readIntInputWithExit(level, 1, currentLevel, true);
        while (command == "Fail") {
            buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("RetryMenu", 1, currentLevel), 4, 0));
            renderer.receiveBuffer(buffer.sendBuffer());
            renderer.output();
            buffer.clear();
            buffer.fetchDrawRequest(generateDrawRequestDataFromString(translator.getTranslation("Title"), 0, 0), generateDrawRequestDataFromString(translator.getTranslation("SelectLevel", currentLevel, 1, currentLevel), 2, 0));
            renderer.receiveBuffer(buffer.sendBuffer());
            renderer.output();
            command = readIntInputWithExit(level, 1, currentLevel, true);
        }
        if (command == "Exit")
            return;

        maze = readLevelMaze(level, fileRW);
        record = readLevelRecord(level, fileRW);
        for (size_t i = 0; i < maze.getRowCount(); i++)
            for (size_t j = 0; j < maze.getColCount(i); j++) {
                if (maze[i][j] == 'O') {
                    currentx = i;
                    currenty = j;
                }
                if (maze[i][j] == 'E') {
                    endx = i;
                    endy = j;
                }
            }
        
        while (currentx != endx && currenty != endy) {
            char ch = _getch();
            switch (ch) {
                case 'w':
                    if (maze[currentx][currenty - 1] != '#') {
                        maze[currentx][currenty--] = ' ';
                        maze[currentx][currenty] = 'O';
                    }
                    break;
                case 'a':
                    if (maze[currentx - 1][currenty] != '#') {
                        maze[currentx--][currenty] = ' ';
                        maze[currentx][currenty] = 'O';
                    }
                    break;
                case 's':
                    if (maze[currentx][currenty + 1] != '#') {
                        maze[currentx][currenty++] = ' ';
                        maze[currentx][currenty] = 'O';
                    }
                    break;
                case 'd':
                    if (maze[currentx + 1][currenty] != '#') {
                        maze[currentx++][currenty] = ' ';
                        maze[currentx][currenty] = 'O';
                    }
                    break;
                default:
                    break;
            }
        }

        return;
    }

    void editMenu(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer, MyFile& fileRW) {
        // Debug starts.
        std::cout << "Edit!" << std::endl;
        Sleep(3000);
        // Debug ends.
        return;
    }

    void selectLanguage(MyBuffer& buffer, MyRenderer& renderer, MyFile& reader) {
        // Debug starts.
        std::cout << "Language!" << std::endl;
        Sleep(3000);
        // Debug ends.
        return;
    }

    void exitGame(MyTranslator& translator, MyBuffer& buffer, MyRenderer& renderer) {
        // Debug starts.
        std::cout << "Exit!" << std::endl;
        Sleep(3000);
        // Debug ends.
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
                case 1:
                    game(translator, genericBuffer, genericRenderer, genericFileRW);
                    break;
                case 2:
                    editMenu(translator, genericBuffer, genericRenderer, genericFileRW);
                    break;
                case 3:
                    selectLanguage(genericBuffer, genericRenderer, genericFileRW);
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