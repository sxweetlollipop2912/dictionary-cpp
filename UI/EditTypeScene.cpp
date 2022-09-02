#include "EditTypeScene.h"
#include "Interaction.h"
#include "SceneManager.h"

static SceneType current_scene = SceneType::EditType;
static App *app;
static const int page_size = 5;
static int page_num, page_cnt;
static int idx, idx2;
static bool data_change;
static Input_Textbox *input_add_boxP;

static void go_back(int dummy) {
	app->scenes.pop_back();
}

static void previous(int dummy) {
	data_change = true;
	page_num = std::max(page_num - 1, 0);
}

static void next(int dummy) {
	data_change = true;
	page_num = std::min(page_num + 1, page_cnt - 1);
}

void add_def(int dummy) {
	data_change = true;
	app->dict.add_def(idx, idx2, input_add_boxP->text);
}

void delete_def(int id) {
	data_change = true;
	app->dict.delete_def(idx, idx2, id);
}


void EditTypeScene(sf::RenderWindow &window, App &_app) {
	app = &_app;
	idx = app->scenes.entry_idx;
	idx2 = app->scenes.entry_idx2;

	sf::Texture texture;
	texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

	Textbox word(app->dict[idx]->word, 40, sf::Color::Red, sf::Vector2f(500, 50),
		sf::Vector2f(200, 50), sf::Color::White);

	Textbox add_box("", 18, sf::Color::Black, sf::Vector2f(50, 125), sf::Vector2f(1050, 100), sf::Color::White);
	add_box.set_outline(sf::Color::Black);
	Input_Textbox input_add_box(add_box, 40, sf::Color::Yellow);
	input_add_box.set_break_line();
    input_add_box.set_idle_outline(sf::Color::Black);
	input_add_boxP = &input_add_box;

	Textbox add_button_box("ADD", 30, sf::Color::Black, sf::Vector2f(1100, 125), sf::Vector2f(50, 50), sf::Color::Cyan);
	Button_Textbox add_button(add_button_box, sf::Color::Yellow);

	texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
	Button_Sprite previous_button(texture, sf::Vector2f(910, 260), sf::Vector2f(40, 40));
	texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
	Button_Sprite next_button(texture, sf::Vector2f(950, 260), sf::Vector2f(40, 40));

	page_cnt = 0;
	while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene) {
		auto defs = app->dict[idx]->types[idx2].defs;
		int n = defs.size();
		int page_cnt = (n - 1) / page_size + 1;

		std::vector<Textbox> textboxs;
		textboxs.push_back(word);

		Button_List<Button_Sprite> del_col;
		texture.loadFromFile(CONST::PATH::IMAGES + "delete.png");

		for (int i = 0; page_num * page_size + i < n; ++i) {
			Textbox def(defs[i], 18, sf::Color::Black, sf::Vector2f(50, 300 + i * 100), sf::Vector2f(1050, 100), sf::Color::White, true);
			def.set_outline(sf::Color::Black);
			textboxs.push_back(def);

			Button_Sprite delete_button(texture, sf::Vector2f(1100, 300 + i * 100), sf::Vector2f(50, 50));
			del_col.add_button(delete_button);
		}

		Interaction interaction;
		interaction.add_button(back_button, go_back);
		interaction.add_button(previous_button, previous);
		interaction.add_button(next_button, next);
		interaction.add_input_textbox(input_add_box);
		interaction.add_button(add_button, add_def);

		del_col.trigger = delete_def;
		data_change = false;

        interaction.add_button_list(del_col);
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