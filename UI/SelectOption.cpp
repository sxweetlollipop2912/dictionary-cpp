#include "SelectOption.h"
#include "SceneManager.h"
#include "../Constants.h"

static App* app_ = nullptr;
static SceneType current_scene = SceneType::SelectOption;

static void searching(int dummy) {
    app_->scenes.push_back(SceneType::Search);
}

static void back(int dummy) {
    app_->scenes.pop_back();
}

static void addingWord(int dummy) {
    app_->scenes.push_back(SceneType::AddWord);
}

static void randomView(int dummy) {
    app_->scenes.entry_idx = app_->dict.random();
    app_->scenes.push_back(SceneType::ViewWord);
}

static void favsList(int dummy) {
    app_->scenes.push_back(SceneType::ViewFavorites);
}

static void searched(int dummy) {
    app_->scenes.push_back(SceneType::ViewHistory);
}

static void multipleChoiceQuestion(int dummy) {
    app_->scenes.push_back(SceneType::MultipleChoice);
}

static void reset(int dummy) {
    app_->reset();
}

void SelectOption(sf::RenderWindow& window, App& _app) {
    app_ = &_app;
    std::vector<Textbox> textboxes;
    Interaction interaction;

    float current_y = CONST::GUI::LARGE_CHAR_SIZE * 3, inc_y = CONST::GUI::LARGE_CHAR_SIZE * 3 / 2;

    sf::Texture texture;
    texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(20, 20), sf::Vector2f(50, 50));
    interaction.add_button(back_button, back);

    Textbox search("Searching", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(100, current_y), sf::Vector2f(400, inc_y), sf::Color::White, true);
    search.align_center();
    Button_Textbox Search(search, sf::Color::Red);
    Search.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(search);
    interaction.add_button(Search, searching);

    Textbox history("Search history", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(700, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox History(history, sf::Color::Red);
    History.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(history);
    interaction.add_button(History, searched);

    current_y += inc_y * 5 / 2;

    Textbox addWord("Adding new words", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(100, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox AddWord(addWord, sf::Color::Red);
    AddWord.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(addWord);
    interaction.add_button(AddWord, addingWord);

    Textbox favorites("Favorites list", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(700, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox Favorites(favorites, sf::Color::Red);
    Favorites.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(favorites);
    interaction.add_button(Favorites, favsList);

    current_y += inc_y * 5 / 2;

    Textbox viewRandom("Viewing random words", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(100, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox ViewRandom(viewRandom, sf::Color::Red);
    ViewRandom.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(viewRandom);
    interaction.add_button(ViewRandom, randomView);

    Textbox MCQ("Multiple choice question", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
        sf::Vector2f(700, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox question(MCQ, sf::Color::Red);
    question.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(MCQ);
    interaction.add_button(question, multipleChoiceQuestion);

    current_y += inc_y * 5 / 2;

    Textbox reset_tb("Reset dictionary", CONST::GUI::LARGE_CHAR_SIZE * 3 / 4, sf::Color::Black,
                     sf::Vector2f(400, current_y), sf::Vector2f(400, inc_y), sf::Color::White);
    Button_Textbox Reset(reset_tb, sf::Color::Red);
    Reset.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(reset_tb);
    interaction.add_button(Reset, reset);

    while (window.isOpen() && !app_->scenes.empty() && app_->scenes.back() == current_scene && !app_->scenes.refresh) {
        window.clear(sf::Color::White);

        for (auto& textbox : textboxes)
            textbox.draw(window, app_->scenes.default_font);
        interaction.draw(window, app_->scenes.default_font);

        window.display();

        auto event = interaction.interact(window);
        app_->scenes.interact(event);
    }
}