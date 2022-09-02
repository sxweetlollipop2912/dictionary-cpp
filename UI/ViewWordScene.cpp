#include "ViewWordScene.h"
#include "Interaction.h"
#include "SceneManager.h"

static const int page_size = 7;

static SceneType current_scene = SceneType::ViewWord;
static App *app;
static int page_num, page_cnt;
static int idx;

static void go_back(int dummy) {
	app->scenes.pop_back();
}

static void previous(int dummy) {
	page_num = std::max(page_num - 1, 0);
}

static void next(int dummy) {
	page_num = std::min(page_num + 1, page_cnt - 1);
}

static void add_to_favorite(int dummy) {
	app->add_favorite(idx);
}

static void delete_word(int dummy) {
	app->dict.erase(idx);
	app->scenes.pop_back();
}

static void edit_word(int dummy) {
	app->scenes.push_back(SceneType::EditWord);
}

void check_high(int &cur_high, std::vector<std::vector<Textbox>> &page, std::vector<Textbox> &textboxs, int add_high) {
	if (cur_high + add_high > 900) {
		page.push_back(textboxs);
		textboxs.clear();
		cur_high = 200;
	}
}

void ViewWordScene(sf::RenderWindow &window, App &_app) {
	app = &_app;

	idx = app->scenes.entry_idx;
	app->add_history(idx);

	sf::Texture texture;
	texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));
	
	Textbox word(app->dict[idx]->word, CONST::GUI::LARGE_CHAR_SIZE, sf::Color::Red, sf::Vector2f(500, 50),
		sf::Vector2f(200, 100), sf::Color::White);

	texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
	Button_Sprite previous_button(texture, sf::Vector2f(910, 160), sf::Vector2f(40, 40));
	texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
	Button_Sprite next_button(texture, sf::Vector2f(950, 160), sf::Vector2f(40, 40));

	Textbox favorite_box("FAVORITE", 30, sf::Color::Black, sf::Vector2f(600, 0), sf::Vector2f(170, 50), sf::Color::Blue);
    favorite_box.set_box_color(sf::Color::Cyan);
    Button_Textbox favorite_button(favorite_box, sf::Color::Yellow);

	Textbox delete_box("DELETE", 30, sf::Color::Black, sf::Vector2f(800, 0), sf::Vector2f(170, 50), sf::Color::Blue);
	delete_box.set_box_color(sf::Color::Red);
    Button_Textbox delete_button(delete_box, sf::Color::Yellow);

	Textbox edit_box("EDIT", 30, sf::Color::Black, sf::Vector2f(1000, 0), sf::Vector2f(170, 50), sf::Color::Blue);
    edit_box.set_box_color(sf::Color::Cyan);
    Button_Textbox edit_button(edit_box, sf::Color::Yellow);

	Interaction interaction;
	interaction.add_button(back_button, go_back);
	interaction.add_button(previous_button, previous);
	interaction.add_button(next_button, next);
	interaction.add_button(favorite_button, add_to_favorite);
	interaction.add_button(delete_button, delete_word);
	interaction.add_button(edit_button, edit_word);

	std::vector<std::vector<Textbox>> page;
	std::vector<Textbox> textboxs;

	int cur_high = 200;
	for (const auto &type : app->dict[idx]->types) {

		check_high(cur_high, page, textboxs, 50);
		Textbox type_textbox("* " + type.type, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(50, cur_high),
			sf::Vector2f(1150, 50), sf::Color::White, true);
		cur_high += 50;
		textboxs.push_back(type_textbox);

		check_high(cur_high, page, textboxs, 50);
		Textbox def_title("Definition", CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(50, cur_high),
			sf::Vector2f(1150, 50), sf::Color::White, true);
		cur_high += 50;
		textboxs.push_back(def_title);

		for (const auto &def : type.defs) {
			check_high(cur_high, page, textboxs, 100);
			Textbox def_textbox(" - " + def, CONST::GUI::MEDIUM_CHAR_SIZE, sf::Color::Black, sf::Vector2f(50, cur_high),
				sf::Vector2f(1150, 100), sf::Color::White, true);
			cur_high += 100;
			textboxs.push_back(def_textbox);
		}
	}

	if (!textboxs.empty())
		page.push_back(textboxs);
	page_num = 0;
	page_cnt = page.size();

	while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene) {
		window.clear(sf::Color::White);

		word.draw(window, app->scenes.default_font);
		if (page_cnt) {
			for (Textbox textbox : page[page_num])
				textbox.draw(window, app->scenes.default_font);
		}
		interaction.draw(window, app->scenes.default_font);
		window.display();

		auto event = interaction.interact(window);
		app->scenes.interact(event);
	}
}