#include "AddWordScene.h"
#include "Interaction.h"

static SceneType current_scene = SceneType::AddWord;
static App *app;
static const int MAX_CHAR_PER_LINE = 60;
static const int LENGTH_LIMIT = MAX_CHAR_PER_LINE * 3;

static Entry entry;
static Input_Textbox *word, *input1, *input2;

static const int opt_type = 0, opt_def = 1, opt_ex = 2;
static int current_opt = -1;

static void after_refresh() {
    app = nullptr;
    word = input1 = input2 = nullptr;
}

static void go_back(int dummy) {
    app->scenes.pop_back();
}

static void insert_word_to_dict(int dummy) {
    auto idx = app->dict.insert(entry);
    app->scenes.pop_back();
    app->scenes.entry_idx = idx;
    app->scenes.push_back(SceneType::ViewWord);
}

static void set_word(int dummy) {
    if (!word->text.empty()) {
        entry.word = word->text;
        app->scenes.refresh = true;
    }
}

static void add_type(int dummy) {
    entry.types.emplace_back();
    current_opt = opt_type;
    app->scenes.refresh = true;
}

static void add_def(int dummy) {
    if (!entry.types.empty()) {
        current_opt = opt_def;
        app->scenes.refresh = true;
    }
}

static void add_example(int dummy) {
    if (!entry.types.empty()) {
        current_opt = opt_ex;
        app->scenes.refresh = true;
    }
}

static void set(int dummy) {
    if (current_opt == opt_type) {
        if (!input1->text.empty()) {
            entry.types.back().type = input1->text;
            current_opt = -1;
            app->scenes.refresh = true;
        }
    }
    else if (current_opt == opt_def) {
        if (!input1->text.empty()) {
            entry.types.back().defs.emplace_back(input1->text);
            current_opt = -1;
            app->scenes.refresh = true;
        }
    }
    else if (current_opt == opt_ex) {
        if (!input1->text.empty()) {
            entry.types.back().examples.emplace_back(input1->text, input2->text);
            current_opt = -1;
            app->scenes.refresh = true;
        }
    }
}

void AddWordScene(sf::RenderWindow &window, App &app_) {
    after_refresh();
    app = &app_;

    std::vector<Textbox> textboxes;
    Interaction interaction;
    std::vector<Button_Sprite> btn_sprites;
    btn_sprites.reserve(2);

    float def_box_y = CONST::GUI::MEDIUM_CHAR_SIZE * 3 * 0.75, inc_y = def_box_y / 3, current_y = inc_y;

    if (entry.word.empty()) {
        float box_x = 900, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2);

        {
            Textbox input_tb({},
                             CONST::GUI::MEDIUM_CHAR_SIZE,
                             sf::Color::Black,
                             sf::Vector2f(pos_x, current_y + def_box_y + inc_y),
                             sf::Vector2f(box_x, def_box_y),
                             sf::Color::White);
            word = new Input_Textbox(input_tb, MAX_CHAR_PER_LINE, sf::Color::Blue);
            word->set_idle_outline(sf::Color::Black);
            word->set_break_line(true);
            interaction.add_input_textbox(*word);
        }

        {
            Textbox box("New word",
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(pos_x, current_y),
                        sf::Vector2f(box_x, def_box_y),
                        sf::Color::White);
            box.align_left();
            textboxes.push_back(box);
        }

        current_y += 2 * (def_box_y + inc_y);
        {
            sf::Texture texture;
            texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
            btn_sprites.emplace_back(texture, sf::Vector2f(pos_x, current_y),
                                     sf::Vector2f(40, 40));
            interaction.add_button(btn_sprites.back(), set_word);
        }
    }
    else {
        {
            float box_x = 900, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2);
            {
                Textbox box("New word: " + (std::string) entry.word,
                            CONST::GUI::MEDIUM_CHAR_SIZE,
                            sf::Color::Black,
                            sf::Vector2f(pos_x, current_y),
                            sf::Vector2f(box_x, def_box_y),
                            sf::Color::White,
                            true);
                box.align_left();
                textboxes.push_back(box);

                current_y += def_box_y + inc_y;
            }
            {
                Textbox box("Current type: " + (entry.types.empty() ? "None" :
                entry.types.back().type.empty() ? "Unknown" :
                (std::string) entry.types.back().type),
                            CONST::GUI::MEDIUM_CHAR_SIZE,
                            sf::Color::Black,
                            sf::Vector2f(pos_x, current_y),
                            sf::Vector2f(box_x, def_box_y),
                            sf::Color::White,
                            true);
                box.align_left();
                textboxes.push_back(box);

                current_y += def_box_y + inc_y;
            }
        }
        if (entry.types.empty()) {
            float box_x = 350, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2;
            Textbox box("Add a type",
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(pos_x, current_y),
                        sf::Vector2f(box_x, def_box_y),
                        sf::Color::White);
            box.align_center();
            Button_Textbox btn(box, sf::Color::Yellow);
            btn.set_idle_outline(sf::Color::Blue);
            textboxes.push_back(box);
            interaction.add_button(btn, add_type);

            current_y += def_box_y + inc_y;
        }
        else {
            {
                float box_x = 350, dist = 30;
                float pos_x2 = CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2;
                float pos_x1 = pos_x2 - dist - box_x;
                float pos_x3 = pos_x2 + dist + box_x;

                {
                    Textbox box("Add a type",
                                CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                                sf::Vector2f(pos_x1, current_y),
                                sf::Vector2f(box_x, def_box_y),
                                sf::Color::White);
                    box.align_center();
                    Button_Textbox btn(box, sf::Color::Yellow);
                    btn.set_idle_outline(sf::Color::Blue);
                    textboxes.push_back(box);
                    interaction.add_button(btn, add_type);
                }
                {
                    Textbox box("Add a definition",
                                CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                                sf::Vector2f(pos_x2, current_y),
                                sf::Vector2f(box_x, def_box_y),
                                sf::Color::White);
                    box.align_center();
                    Button_Textbox btn(box, sf::Color::Yellow);
                    btn.set_idle_outline(sf::Color::Blue);
                    textboxes.push_back(box);
                    interaction.add_button(btn, add_def);
                }
                {
                    Textbox box("Add an example",
                                CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                                sf::Vector2f(pos_x3, current_y),
                                sf::Vector2f(box_x, def_box_y),
                                sf::Color::White);
                    box.align_center();
                    Button_Textbox btn(box, sf::Color::Yellow);
                    btn.set_idle_outline(sf::Color::Blue);
                    textboxes.push_back(box);
                    interaction.add_button(btn, add_example);
                }

                current_y += def_box_y + inc_y;
            }

            if (current_opt != -1) {
                float box_x = 900, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2);
                {
                    float box_y = current_opt == opt_type ? def_box_y : def_box_y * 2;
                    int length_limit = current_opt == opt_type ? MAX_CHAR_PER_LINE : LENGTH_LIMIT;
                    {
                        Textbox input_tb({},
                                         CONST::GUI::MEDIUM_CHAR_SIZE,
                                         sf::Color::Black,
                                         sf::Vector2f(pos_x, current_y + def_box_y + inc_y),
                                         sf::Vector2f(box_x, box_y),
                                         sf::Color::White,
                                         true);
                        input1 = new Input_Textbox(input_tb, length_limit, sf::Color::Blue);
                        input1->set_idle_outline(sf::Color::Black);
                        input1->set_break_line(true);
                        interaction.add_input_textbox(*input1);
                    }
                    {
                        auto msg = current_opt == opt_type ? "New type" : current_opt == opt_def ? "New definition"
                                                                                                 : "New example";
                        Textbox box(msg,
                                    CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                                    sf::Vector2f(pos_x, current_y),
                                    sf::Vector2f(box_x, def_box_y),
                                    sf::Color::White);
                        box.align_left();
                        textboxes.push_back(box);
                    }
                    current_y += box_y + def_box_y + 2 * inc_y;
                }
                if (current_opt == opt_ex) {
                    float box_y = def_box_y * 2;
                    int length_limit = LENGTH_LIMIT;
                    {
                        Textbox input_tb({},
                                         CONST::GUI::MEDIUM_CHAR_SIZE,
                                         sf::Color::Black,
                                         sf::Vector2f(pos_x, current_y + def_box_y + inc_y),
                                         sf::Vector2f(box_x, box_y),
                                         sf::Color::White,
                                         true);
                        input2 = new Input_Textbox(input_tb, length_limit, sf::Color::Blue);
                        input2->set_idle_outline(sf::Color::Black);
                        input2->set_break_line(true);
                        interaction.add_input_textbox(*input2);
                    }
                    {
                        auto msg = "Example translation";
                        Textbox box(msg,
                                    CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                                    sf::Vector2f(pos_x, current_y),
                                    sf::Vector2f(box_x, def_box_y),
                                    sf::Color::White);
                        box.align_left();
                        textboxes.push_back(box);
                    }
                    current_y += box_y + def_box_y + 2 * inc_y;
                }
                {
                    sf::Texture texture;
                    texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
                    btn_sprites.emplace_back(texture, sf::Vector2f(pos_x, current_y),
                                             sf::Vector2f(40, 40));
                    interaction.add_button(btn_sprites.back(), set);

                    current_y += def_box_y + inc_y;
                }
            }
        }

        {
            current_y += def_box_y + inc_y;

            float box_x = 500, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - box_x / 2;
            Textbox box("Add word to dictionary",
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(pos_x, current_y),
                        sf::Vector2f(box_x, def_box_y),
                        sf::Color::White);
            box.align_center();
            Button_Textbox btn(box, sf::Color::Yellow);
            btn.set_idle_outline(sf::Color::Blue);
            textboxes.push_back(box);
            interaction.add_button(btn, insert_word_to_dict);

            current_y += def_box_y + inc_y;
        }
    }

    {
        sf::Texture texture;
        texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
        btn_sprites.emplace_back(texture, sf::Vector2f(30, 30),
                                 sf::Vector2f(40, 40));
        interaction.add_button(btn_sprites.back(), go_back);
    }

    while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !app->scenes.refresh) {
        window.clear(sf::Color::White);

        for (auto &textbox: textboxes)
            textbox.draw(window, app->scenes.default_font);
        interaction.draw(window, app->scenes.default_font);

        window.display();

        auto event = interaction.interact(window);
        app->scenes.interact(event);
    }

    delete word;
    delete input1;
    delete input2;
}