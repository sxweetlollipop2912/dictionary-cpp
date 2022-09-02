#include "MultipleChoiceScene.h"
#include "Interaction.h"

static SceneType current_scene = SceneType::MultipleChoice;
static App *app = nullptr;
static const int MAX_CHAR_PER_LINE = 70;

static bool current_option = false;
static int opts[] = {-1, -1, -1, -1};
static std::string opt_defs[] = {"", "", "", ""};
static int answer = -1;
static int choice = -1;
static DictType last_type = DictType::Unknown;

static std::vector<std::string> win_msg = {"Keep it up!", "Wow, didn't expect that!",
                                           "Never gonna give it up, do you? :)", "At last!", "You may just be lucky :)",
                                           "Very well done!", "That is... correct!", "That is not... incorrect!",
                                           "You should be proud of yourself ^^"};
static std::vector<std::string> lose_msg = {"Better luck next time!", "Down on your luck, aren't you?",
                                            "Come on, once more?", "Your hand was shaking, right? Right?",
                                            "Is it too hard for you? :(",
                                            "I thought you would never gonna let me down... :(",
                                            "No pain no gain. Or so they say...", "Again? :)"};

static void go_back(int dummy) {
    app->scenes.pop_back();
}

static bool is_guessing_word() {
    return !current_option;
}

static bool is_guessing_def() {
    return !is_guessing_word();
}

static void again(int dummy) {
    answer = choice = -1;
    app->scenes.refresh = true;
}

static void switch_to_word(int dummy) {
    if (!is_guessing_word()) {
        current_option = !current_option;
        again(dummy);
    }
}

static void switch_to_def(int dummy) {
    if (!is_guessing_def()) {
        current_option = !current_option;
        again(dummy);
    }
}

static void show_answer(int choice_) {
    if (choice == -1) {
        choice = choice_;
        app->scenes.refresh = true;
    }
}

void MultipleChoiceScene(sf::RenderWindow &window, App &app_) {
    app = &app_;

    if (app->dict.type() != last_type) {
        answer = choice = -1;
    }
    last_type = app->dict.type();

    std::vector<Textbox> textboxes;
    Interaction interaction;

    float current_y = 0, box_y = CONST::GUI::MEDIUM_CHAR_SIZE * 3 * 0.75, inc_y = box_y / 3;

    {
        float box_x = 350;
        Textbox box("Guess word by a definition",
                    CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                    sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - box_x - 30, (current_y += box_y + inc_y)),
                    sf::Vector2f(box_x, box_y),
                    is_guessing_word() ? sf::Color::Cyan : sf::Color::White);
        box.align_center();
        Button_Textbox btn(box, sf::Color::Yellow);
        btn.set_idle_outline(sf::Color::Blue);
        textboxes.push_back(box);
        interaction.add_button(btn, switch_to_word);
    }

    {
        float box_x = 350;
        Textbox box("Guess definition by a word",
                    CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                    sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 + 30, current_y),
                    sf::Vector2f(box_x, box_y),
                    is_guessing_def() ? sf::Color::Cyan : sf::Color::White);
        box.align_center();
        Button_Textbox btn(box, sf::Color::Yellow);
        btn.set_idle_outline(sf::Color::Blue);
        textboxes.push_back(box);
        interaction.add_button(btn, switch_to_def);
    }

    if (answer == -1) {
        for (int i = 0; i < 4; i++) {
            opts[i] = app->dict.random();
            bool check = true;
            for (int j = 0; j < i && check; j++)
                check = opts[j] != opts[i];
            if (!check) --i;
            else {
                opt_defs[i] = app->dict[opts[i]]->random_def();
                if (opt_defs[i].size() > MAX_CHAR_PER_LINE * 3)
                    --i;
            }
        }

        answer = rand() % 4;
    }

    {
        float box_x = 900;
        float box_yy = is_guessing_word() ? box_y : box_y * 2;
        auto msg = is_guessing_word() ? (std::string) app->dict[opts[answer]]->word :
                   opt_defs[answer];
        Textbox box(msg,
                    is_guessing_word() ? CONST::GUI::LARGE_CHAR_SIZE : CONST::GUI::MEDIUM_CHAR_SIZE,
                    sf::Color::Black,
                    sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2), (current_y += box_y + inc_y)),
                    sf::Vector2f(box_x, box_yy),
                    sf::Color::White,
                    true);
        box.align_center();
        textboxes.push_back(box);

        if (is_guessing_def()) current_y += box_y;
    }

    {
        if (is_guessing_word()) current_y -= box_y;

        Button_List<Button_Textbox> list;
        list.trigger = show_answer;
        for (int i = 0; i < 4; i++) {
            float box_x = 900;
            float box_yy = is_guessing_word() ? box_y * 2 : box_y;
            auto color = choice == -1 ? sf::Color::White :
                         i == answer ? sf::Color::Green :
                         i == choice ? sf::Color::Red :
                         sf::Color::White;
            auto msg = is_guessing_word() ? opt_defs[i] : (std::string) app->dict[opts[i]]->word;
            Textbox box(msg,
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2),
                                     (current_y += box_yy + inc_y)),
                        sf::Vector2f(box_x, box_yy),
                        color,
                        true);
            box.align_center();
            Button_Textbox btn(box, sf::Color::Yellow);
            btn.set_idle_outline(sf::Color::Blue);
            list.add_button(btn);
        }
        interaction.add_button_list(list);

        if (is_guessing_word()) current_y += box_y;
    }

    if (choice != -1) {
        {
            float box_x = 100;
            auto msg = choice == answer ? win_msg[rand() % win_msg.size()] : lose_msg[rand() % lose_msg.size()];
            Textbox box(msg,
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2), (current_y += box_y + inc_y)),
                        sf::Vector2f(box_x, box_y), sf::Color::White);
            box.align_center();
            textboxes.push_back(box);
        }
        {
            float box_x = 100;
            Textbox box("Again",
                        CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                        sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2), (current_y += box_y + inc_y)),
                        sf::Vector2f(box_x, box_y),
                        is_guessing_word() ? sf::Color::Cyan : sf::Color::White);
            box.align_center();
            Button_Textbox btn(box, sf::Color::Yellow);
            btn.set_idle_outline(sf::Color::Blue);
            textboxes.push_back(box);
            interaction.add_button(btn, again);
        }
    }

    sf::Texture texture;
    texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(30, 30), sf::Vector2f(40, 40));
    interaction.add_button(back_button, go_back);


    while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !app->scenes.refresh) {
        window.clear(sf::Color::White);

        for (auto &textbox: textboxes)
            textbox.draw(window, app->scenes.default_font);
        interaction.draw(window, app->scenes.default_font);

        window.display();

        auto event = interaction.interact(window);
        app->scenes.interact(event);
    }
}
