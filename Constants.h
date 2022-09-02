#pragma once

#include <SFML/Graphics.hpp>

#include "DataStructures/Set.h"

namespace CONST {
    namespace CHAR {
        const Set<char> CHAR_SEPARATOR = {' ', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+',
                                          '{', '}', '[', ']', '|', '\\', ';', ':', '\"', '\'', '<', ',', '>', '.', '?',
                                          '/', '~', '`'};
        const char DUMMY_SPACE = 1;
        const char DUMMY_ENDL = 2;
        const char DUMMY_EMPTY= 6;

        const char NEW_WORD = '@';
        const char TYPE = '*';
        const char DEF = '-';
        const char EXAMPLE = '=';
        const char EX_MEANING = '+';
    }

    namespace PATH {
        const std::string ASSETS = "./assets/";
        const std::string FONTS = ASSETS + (const std::string) "fonts/";
        const std::string DEFAULT_FONT = FONTS + (const std::string) "arial.ttf";
        const std::string IMAGES = ASSETS + (const std::string) "images/";

        const std::string SAVES = "./saves/";
        const std::string EE_SAVES = SAVES + (std::string) "EE/";
        const std::string EV_SAVES = SAVES + (std::string) "EV/";
        const std::string VE_SAVES = SAVES + (std::string) "VE/";
        const std::string SLANG_SAVES = SAVES + (std::string) "Slang/";
        const std::string EMO_SAVES = SAVES + (std::string) "Emo/";
        const std::string TEST_SAVES = SAVES + (std::string) "Test/";
        const std::string DICT_SAVE_FILENAME = "dict.txt";
        const std::string FAV_SAVE_FILENAME = "favs.txt";
        const std::string HISTORY_SAVE_FILENAME = "history.txt";

        const std::string DATA = "./data/";
        const std::string EV_DICT = DATA + (std::string) "EV.txt";
        const std::string VE_DICT = DATA + (std::string) "VE.txt";
        const std::string EE_DICT = DATA + (std::string) "EE.txt";
        const std::string SLANG_DICT = DATA + (std::string) "Slang.txt";
        const std::string EMO_DICT = DATA + (std::string) "Emo.txt";
        const std::string TEST_DICT = DATA + (std::string) "Test.txt";
    }

    namespace GUI {
        const int WINDOW_HEIGHT = 900;
        const int WINDOW_WIDTH = 1200;

        const int MEDIUM_CHAR_SIZE = WINDOW_HEIGHT / 37;
        const int LARGE_CHAR_SIZE = MEDIUM_CHAR_SIZE * 1.7;
        const int SMALL_CHAR_SIZE = MEDIUM_CHAR_SIZE * 3 / 5;
    }
}