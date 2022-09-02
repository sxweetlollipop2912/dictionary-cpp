#include "HistoryScene.h"
#include "Interaction.h"
#include "SceneManager.h"

static const int page_size = 7;

static SceneType current_scene = SceneType::ViewHistory;
static App *app;
static int page_num, page_cnt;
static bool data_change;
static std::vector <Dict::size_type> history;

static void go_back(int dummy) {
    app->scenes.pop_back();
}

static void previous(int dummy) {
    page_num = std::max(page_num - 1, 0);
    data_change = true;
}

static void next(int dummy) {
    page_num = std::min(page_num + 1, page_cnt - 1);
    data_change = true;
}

static void view_word(int i) {
    app->scenes.entry_idx = history[i + page_num * page_size];
    app->scenes.push_back(SceneType::ViewWord);
}

void HistoryScene(sf::RenderWindow &window, App &_app) {
    app = &_app;

    float offset_x = 50;

    sf::Texture texture;
    texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(30, 30), sf::Vector2f(40, 40));

    Textbox history_title("HISTORY", CONST::GUI::LARGE_CHAR_SIZE, sf::Color::Red, sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - 200 / 2, 50), sf::Vector2f(200, 100), sf::Color::White);

    texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
    Button_Sprite previous_button(texture, sf::Vector2f(offset_x + 910, 160), sf::Vector2f(40, 40));
    texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
    Button_Sprite next_button(texture, sf::Vector2f(offset_x + 950, 160), sf::Vector2f(40, 40));

    Textbox st_title("ST", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 50, 200), sf::Vector2f(200, 80), sf::Color::White);
    st_title.set_outline(sf::Color::Black);
    Textbox word_title("WORD", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 250, 200), sf::Vector2f(800, 80), sf::Color::White);
    word_title.set_outline(sf::Color::Black);

    page_num = 0;
    while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene) {
        history = app->history();
        int n = history.size();
        page_cnt = (n - 1) / page_size + 1;
        page_num = std::min(page_num, page_cnt - 1);

        std::string page_num_string;
        if (n > 0)
            page_num_string = std::to_string(page_num + 1) + "/" + std::to_string(page_cnt);
        else
            page_num_string = "0/0";
        Textbox page_num_text(page_num_string, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 990, 160), sf::Vector2f(60, 40), sf::Color::White);

        std::vector<Textbox> textboxs;
        textboxs.push_back(history_title);
        textboxs.push_back(st_title);
        textboxs.push_back(word_title);
        textboxs.push_back(page_num_text);

        Interaction interaction;
        interaction.add_button(back_button, go_back);
        interaction.add_button(previous_button, previous);
        interaction.add_button(next_button, next);

        Button_List<Button_Textbox> word_col;

        for (int i = 0; page_num * page_size + i < n && i < page_size; ++i) {
            Textbox st(std::to_string(page_num * page_size + i + 1), CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                       sf::Vector2f(offset_x + 50, 280 + i * 80), sf::Vector2f(200, 80), sf::Color::White);
            st.set_outline(sf::Color::Black);
            textboxs.push_back(st);

            Textbox word(app->dict[history[page_num * page_size + i]]->word, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                         sf::Vector2f(offset_x + 250, 280 + i * 80), sf::Vector2f(800, 80), sf::Color::White);
            word.set_outline(sf::Color::Black);
            textboxs.push_back(word);
            Button_Textbox btn(word, sf::Color::Cyan);
            btn.set_idle_outline(sf::Color::Black);
            word_col.add_button(btn);
        }
        word_col.trigger = view_word;
        interaction.add_button_list(word_col);

        data_change = false;
        while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !data_change) {
            window.clear(sf::Color::White);

            for (Textbox textbox : textboxs)
                textbox.draw(window, app->scenes.default_font);
            interaction.draw(window, app->scenes.default_font);

            window.display();

            auto event = interaction.interact(window);
            app->scenes.interact(event);
        }
    }
}