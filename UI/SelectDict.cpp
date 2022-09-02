#include <iostream>

#include "SelectDict.h"
#include "SceneManager.h"

static App* app_ = nullptr;
static SceneType current_scene = SceneType::SelectDict;

static void change_scene(int dummy) {
    app_->load();
    app_->scenes.push_back(SceneType::SelectOption);
}

static void EE_dict(int dummy) {
    app_->save();
    app_->unload();
    app_->set_dict_type(DictType::EE);
    change_scene(dummy);
}

static void EV_dict(int dummy) {
    app_->save();
    app_->unload();
    app_->set_dict_type(DictType::EV);
    change_scene(dummy);
}

static void VE_dict(int dummy) {
    app_->save();
    app_->unload();
    app_->set_dict_type(DictType::VE);
    change_scene(dummy);
}

static void emo_dict(int dummy) {
    app_->save();
    app_->unload();
    app_->set_dict_type(DictType::Emo);
    change_scene(dummy);
}

static void slang_dict(int dummy) {
    app_->save();
    app_->unload();
    app_->set_dict_type(DictType::Slang);
    change_scene(dummy);
}

void SelectDict(sf::RenderWindow& window, App& _app) {
    app_ = &_app;
    std::vector<Textbox> textboxes;
    Interaction interaction;

    float current_y = CONST::GUI::LARGE_CHAR_SIZE, inc_y = CONST::GUI::LARGE_CHAR_SIZE;

    float box_x = 100;
    Textbox title("DICTIONARY", CONST::GUI::LARGE_CHAR_SIZE, sf::Color::Magenta,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, current_y), sf::Vector2f(100, inc_y), sf::Color::White, true);
    title.align_center();
    textboxes.push_back(title);

    current_y += 2 * inc_y;

    box_x = 150;

    Textbox ENGtoVIE("Eng-Vie", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, (current_y += inc_y)), sf::Vector2f(150, inc_y), sf::Color::White);
    ENGtoVIE.align_center();
    Button_Textbox EngToVie(ENGtoVIE, sf::Color::Red);
    EngToVie.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(ENGtoVIE);
    interaction.add_button(EngToVie, EV_dict);

    current_y += 2 * inc_y;

    Textbox VIEtoENG("Vie-Eng", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, (current_y += inc_y)), sf::Vector2f(150, inc_y), sf::Color::White);
    VIEtoENG.align_center();
    Button_Textbox VieToEng(VIEtoENG, sf::Color::Red);
    VieToEng.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(VIEtoENG);
    interaction.add_button(VieToEng, VE_dict);

    current_y += 2 * inc_y;

    Textbox ENGtoENG("Eng-Eng", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, (current_y += inc_y)), sf::Vector2f(150, inc_y), sf::Color::White);
    ENGtoENG.align_center();
    Button_Textbox EngToEng(ENGtoENG, sf::Color::Red);
    EngToEng.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(ENGtoENG);
    interaction.add_button(EngToEng, EE_dict);

    current_y += 2 * inc_y;

    Textbox SLANG("Slang", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, (current_y += inc_y)), sf::Vector2f(150, inc_y), sf::Color::White);
    SLANG.align_center();
    Button_Textbox slang(SLANG, sf::Color::Red);
    slang.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(SLANG);
    interaction.add_button(slang, slang_dict);

    current_y += 2 * inc_y;

    Textbox EMOJI("Emoji", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2, (current_y += inc_y)), sf::Vector2f(150, inc_y), sf::Color::White);
    EMOJI.align_center();
    Button_Textbox emoji(EMOJI, sf::Color::Red);
    emoji.set_idle_outline(sf::Color::Blue);
    textboxes.push_back(EMOJI);
    interaction.add_button(emoji, emo_dict);

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
