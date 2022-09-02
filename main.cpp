#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdlib>

#include "App.h"
#include "Enums.h"
#include "UI/MultipleChoiceScene.h"
#include "UI/AddWordScene.h"
#include "UI/SceneManager.h"
#include "UI/SelectDict.h"
#include "UI/FavoriteScene.h"
#include "UI/HistoryScene.h"
#include "UI/SelectOption.h"
#include "UI/SearchScene.h"
#include "UI/ViewWordScene.h"
#include "UI/EditWordScene.h"
#include "UI/EditTypeScene.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;


int main() {
    srand(time(nullptr));

    auto first_scene = SceneType::SelectDict;
    App app;

    sf::RenderWindow window(sf::VideoMode(CONST::GUI::WINDOW_WIDTH, CONST::GUI::WINDOW_HEIGHT), "Dictionary",
                            sf::Style::Close);
    app.scenes.push_back(first_scene);

    while (!app.scenes.empty()) {
        app.scenes.refresh = false;
        auto sceneNow = app.scenes.back();

        if (sceneNow == SceneType::AddWord) {
            AddWordScene(window, app);
        }
        else if (sceneNow == SceneType::MultipleChoice) {
            MultipleChoiceScene(window, app);
        }
        else if (sceneNow == SceneType::SelectDict) {
            SelectDict(window, app);
        }
        else if (sceneNow == SceneType::SelectOption) {
            SelectOption(window, app);
        }
        else if (sceneNow == SceneType::ViewFavorites) {
            FavoriteScene(window, app);
        }
        else if (sceneNow == SceneType::ViewHistory) {
            HistoryScene(window, app);
        }
        else if (sceneNow == SceneType::Search) {
            SearchScene(window, app);
        }
        else if (sceneNow == SceneType::ViewWord) {
            ViewWordScene(window, app);
        }
        else if (sceneNow == SceneType::EditWord) {
            EditWordScene(window, app);
        }
        else if (sceneNow == SceneType::EditType) {
            EditTypeScene(window, app);
        }
        else {
            app.scenes.pop_back();
        }
    }

    {
        auto t1 = high_resolution_clock::now();
        app.save();
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "Saving time :" << ms_double.count() << "ms\n";
    }
}