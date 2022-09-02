#include "SearchScene.h"
#include "Interaction.h"

static const int PAGE_SIZE = 7;
static const int MAX_PAGE_CNT = 10;

static SceneType current_scene = SceneType::Search;
static App *app;
static const int MAX_CHAR_PER_LINE = 70;
static const int LENGTH_LIMIT = MAX_CHAR_PER_LINE * 3;
static int page_num, page_cnt;

static Entry entry;
static Input_Textbox *input;
static std::vector <Dict::size_type> res;

static bool current_opt = true;
static bool showing_res = false;
static bool data_change;
static std::string current_query;

static void back_from_show_res(int dummy) {
    showing_res = false;
    res.clear();
    app->scenes.refresh = true;
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
    showing_res = false;
    app->scenes.refresh = true;
    res.clear();
    app->scenes.entry_idx = res[i + page_num * PAGE_SIZE];
    app->scenes.push_back(SceneType::ViewWord);
}

static void ShowResult(sf::RenderWindow &window) {
    float offset_x = 50;

    sf::Texture texture;
    texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
    Button_Sprite back_button(texture, sf::Vector2f(30, 30), sf::Vector2f(40, 40));

    Textbox title("Results for \"" + current_query + "\"",
                  CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Red,
                  sf::Vector2f(CONST::GUI::WINDOW_WIDTH / 2 - 200 / 2, 50),
                  sf::Vector2f(200, 100), sf::Color::White);
    title.align_center();

    texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
    Button_Sprite previous_button(texture, sf::Vector2f(offset_x + 910, 160), sf::Vector2f(40, 40));
    texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
    Button_Sprite next_button(texture, sf::Vector2f(offset_x + 950, 160), sf::Vector2f(40, 40));

    Textbox st_title("ST", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 50, 200), sf::Vector2f(200, 80), sf::Color::White);
    st_title.set_outline(sf::Color::Black);
    Textbox word_title("WORD", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 250, 200), sf::Vector2f(800, 80), sf::Color::White);
    word_title.set_outline(sf::Color::Black);

    page_num = 0;
    while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !app->scenes.refresh) {
        int n = (int) res.size();
        page_cnt = (n - 1) / PAGE_SIZE + 1;
        page_num = std::min(page_num, page_cnt - 1);

        std::string page_num_string;
        if (n > 0)
            page_num_string = std::to_string(page_num + 1) + "/" + std::to_string(page_cnt);
        else
            page_num_string = "0/0";
        Textbox page_num_text(page_num_string, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(offset_x + 990, 160), sf::Vector2f(60, 40), sf::Color::White);

        std::vector<Textbox> textboxs;
        textboxs.push_back(title);
        textboxs.push_back(st_title);
        textboxs.push_back(word_title);
        textboxs.push_back(page_num_text);

        Interaction interaction;
        interaction.add_button(back_button, back_from_show_res);
        interaction.add_button(previous_button, previous);
        interaction.add_button(next_button, next);

        Button_List<Button_Textbox> word_col;

        for (int i = 0; page_num * PAGE_SIZE + i < n && i < PAGE_SIZE; ++i) {
            Textbox st(std::to_string(page_num * PAGE_SIZE + i + 1), CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                       sf::Vector2f(offset_x + 50, 280 + i * 80), sf::Vector2f(200, 80), sf::Color::White);
            st.set_outline(sf::Color::Black);
            textboxs.push_back(st);

            Textbox word(app->dict[res[page_num * PAGE_SIZE + i]]->word, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
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
        while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !data_change && !app->scenes.refresh) {
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

static void after_refresh() {
    app = nullptr;
    input = nullptr;
    res.clear();
}

static void go_back(int dummy) {
    app->scenes.pop_back();
}

static bool is_searching_word() {
    return current_opt;
}

static bool is_searching_def() {
    return !is_searching_word();
}

static void search(int dummy) {
    current_query = input->text;
    if (!current_query.empty()) {
        if (is_searching_word()) {
            res = app->dict.search_word(current_query, PAGE_SIZE * MAX_PAGE_CNT);
        } else if (is_searching_def()) {
            res = app->dict.search_def(current_query, PAGE_SIZE * MAX_PAGE_CNT);
        }
        showing_res = true;
        app->scenes.refresh = true;
    }
}

static void switch_option(int dummy) {
    current_opt = !current_opt;
    app->scenes.refresh = true;
}

void SearchScene(sf::RenderWindow &window, App &app_) {
    if (showing_res) {
        ShowResult(window);
        return;
    }

    after_refresh();
    app = &app_;

    std::vector<Textbox> textboxes;
    Interaction interaction;
    std::vector<Button_Sprite> btn_sprites;
    btn_sprites.reserve(2);

    float def_box_y = CONST::GUI::MEDIUM_CHAR_SIZE * 3 * 0.75, inc_y = def_box_y / 3, current_y = inc_y;

    float box_x = 900, pos_x = CONST::GUI::WINDOW_WIDTH / 2 - (box_x / 2);

    {
        float box_y = def_box_y * 2;
        Textbox input_tb({},
                         CONST::GUI::MEDIUM_CHAR_SIZE,
                         sf::Color::Black,
                         sf::Vector2f(pos_x, current_y + def_box_y + inc_y),
                         sf::Vector2f(box_x, box_y),
                         sf::Color::White,
                         true);
        input = new Input_Textbox(input_tb, LENGTH_LIMIT, sf::Color::Blue);
        input->set_idle_outline(sf::Color::Black);
        input->set_break_line(true);
        interaction.add_input_textbox(*input);
    }

    {
        Textbox box(is_searching_word() ? "Searching by word" : "Searching by definition",
                    CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
                    sf::Vector2f(pos_x, current_y),
                    sf::Vector2f(box_x / 2.5, def_box_y),
                    sf::Color::White);
        box.align_center();
        Button_Textbox btn(box, sf::Color::Yellow);
        btn.set_idle_outline(sf::Color::Blue);
        textboxes.push_back(box);
        interaction.add_button(btn, switch_option);
    }

    current_y += 3 * def_box_y + 2 * inc_y;
    {
        sf::Texture texture;
        texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
        btn_sprites.emplace_back(texture, sf::Vector2f(pos_x, current_y),
                                 sf::Vector2f(40, 40));
        interaction.add_button(btn_sprites.back(), search);
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

    delete input;
}