#pragma once

#include "../Dictionary/Dict.h"

#include <SFML/Graphics.hpp>

enum class SceneType {
    MockScene1,
    MockScene2,
    SelectDict,
    SelectOption,
    Search,
    ViewWord,
    EditWord,
    AddWord,
    MultipleChoice,
    ViewHistory,
    ViewFavorites,
	EditType
};

class SceneManager : public std::vector<SceneType> {
public:
    Dict::size_type entry_idx, entry_idx2;
    bool refresh = false;
    sf::Font default_font;

    SceneManager();

    void interact(sf::Event event);
};
