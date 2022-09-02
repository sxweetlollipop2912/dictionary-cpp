#include "SceneManager.h"
#include "../Constants.h"

void SceneManager::interact(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        this->clear();
    }
}

SceneManager::SceneManager() {
    entry_idx = -1;
    default_font.loadFromFile(CONST::PATH::DEFAULT_FONT);
}
