#include "FavoriteScene.h"
#include "Interaction.h"
#include "SceneManager.h"

static const int page_size = 7;

static App *app;
static int page_num, page_cnt;
static bool data_change;
static std::vector <Dict::size_type> favorites;

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

static void delete_favorite(int i) {
	app->remove_favorite(favorites[i + page_num * page_size]);
    data_change = true;
}

static void view_word(int i) {
    app->scenes.entry_idx = favorites[i + page_num * page_size];
    app->scenes.push_back(SceneType::ViewWord);
}

void FavoriteScene(sf::RenderWindow &window, App &_app) {
	app = &_app;

	sf::Texture texture;
	texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(30, 30), sf::Vector2f(40, 40));

	Textbox favorite_title("FAVORITE", CONST::GUI::LARGE_CHAR_SIZE, sf::Color::Red, sf::Vector2f(500, 50), sf::Vector2f(200, 100), sf::Color::White);

	texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
	Button_Sprite previous_button(texture, sf::Vector2f(910, 160), sf::Vector2f(40, 40));
	texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
	Button_Sprite next_button(texture, sf::Vector2f(950, 160), sf::Vector2f(40, 40));

	Textbox st_title("ST", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(50, 200), sf::Vector2f(200, 80), sf::Color::White);
	st_title.set_outline(sf::Color::Black);
	Textbox word_title("WORD", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(250, 200), sf::Vector2f(800, 80), sf::Color::White);
	word_title.set_outline(sf::Color::Black);

	page_num = 0;
	while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == SceneType::ViewFavorites) {
		favorites = app->favorite();
		int n = favorites.size();
		page_cnt = (n - 1) / page_size + 1;
		page_num = std::min(page_num, page_cnt - 1);

		std::string page_num_string;
		if (n > 0)
			page_num_string = std::to_string(page_num + 1) + "/" + std::to_string(page_cnt);
		else
			page_num_string = "0/0";
		Textbox page_num_text(page_num_string, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(990, 160), sf::Vector2f(60, 40), sf::Color::White);

		std::vector<Textbox> textboxs;
		textboxs.push_back(favorite_title);
		textboxs.push_back(st_title);
		textboxs.push_back(word_title);
		textboxs.push_back(page_num_text);

		Interaction interaction;
		interaction.add_button(back_button, go_back);
		interaction.add_button(previous_button, previous);
		interaction.add_button(next_button, next);

        Button_List<Button_Textbox> word_col;
		Button_List<Button_Sprite> del_col;
		texture.loadFromFile(CONST::PATH::IMAGES + "delete.png");

		for (int i = 0; page_num * page_size + i < n && i < page_size; ++i) {
			Textbox st(std::to_string(page_num * page_size + i + 1), CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
				sf::Vector2f(50, 280 + i * 80), sf::Vector2f(200, 80), sf::Color::White);
			st.set_outline(sf::Color::Black);
			textboxs.push_back(st);

			Textbox word(app->dict[favorites[page_num * page_size + i]]->word, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black,
				sf::Vector2f(250, 280 + i * 80), sf::Vector2f(800, 80), sf::Color::White);
			word.set_outline(sf::Color::Black);
			textboxs.push_back(word);
            Button_Textbox btn(word, sf::Color::Cyan);
            btn.set_idle_outline(sf::Color::Black);
            word_col.add_button(btn);

			Button_Sprite delete_button(texture, sf::Vector2f(1050, 280 + i * 80), sf::Vector2f(80, 80));
			del_col.add_button(delete_button);
		}

        /// Somehow fixed delete image not showing.
        if (page_num * page_size < n)
        {
            Button_Sprite delete_button(texture, sf::Vector2f(1050, 280), sf::Vector2f(80, 80));
            del_col.add_button(delete_button);
        }

		del_col.trigger = delete_favorite;
        word_col.trigger = view_word;
		interaction.add_button_list(del_col);
        interaction.add_button_list(word_col);

		data_change = false;
		while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == SceneType::ViewFavorites && !data_change) {
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