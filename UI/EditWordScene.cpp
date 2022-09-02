#include "EditWordScene.h"
#include "Interaction.h"
#include "SceneManager.h"

static SceneType current_scene = SceneType::EditWord;
static App *app;
static const int page_size = 10;
static int page_num, page_cnt;
static int idx;
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

void add_type(int dummy) {
	data_change = true;
	app->dict.add_type(idx, input_add_boxP->text);
}

void delete_type(int id) {
	data_change = true;
	app->dict.delete_type(idx, id);
}

void go_to_type(int id) {
	app->scenes.entry_idx2 = id;
	app->scenes.push_back(SceneType::EditType);
}

void EditWordScene(sf::RenderWindow &window, App &_app) {
	app = &_app;
	idx = app->scenes.entry_idx;

	sf::Texture texture;
	texture.loadFromFile(CONST::PATH::IMAGES + "go_back.png");
	Button_Sprite back_button(texture, sf::Vector2f(10, 5), sf::Vector2f(40, 40));

	Textbox word(app->dict[idx]->word, 40, sf::Color::Red, sf::Vector2f(500, 50),
		sf::Vector2f(200, 50), sf::Color::White);

	Textbox add_box("", 18, sf::Color::Black, sf::Vector2f(50, 125), sf::Vector2f(1050, 50), sf::Color::White);
	add_box.set_outline(sf::Color::Black);
	Input_Textbox input_add_box(add_box, 40, sf::Color::Yellow);
    input_add_box.set_idle_outline(sf::Color::Black);
	input_add_boxP = &input_add_box;

	Textbox add_button_box("ADD", 30, sf::Color::Black, sf::Vector2f(1100, 125), sf::Vector2f(50, 50), sf::Color::Cyan);
	Button_Textbox add_button(add_button_box, sf::Color::Yellow);

	texture.loadFromFile(CONST::PATH::IMAGES + "previous.jpg");
	Button_Sprite previous_button(texture, sf::Vector2f(910, 210), sf::Vector2f(40, 40));
	texture.loadFromFile(CONST::PATH::IMAGES + "next.jpg");
	Button_Sprite next_button(texture, sf::Vector2f(950, 210), sf::Vector2f(40, 40));

	page_cnt = 0;
	while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene) {
		auto types = app->dict[idx]->types;
		int n = types.size();
		int page_cnt = (n - 1) / page_size + 1;

		Button_List<Button_Textbox> access_col;
		Button_List<Button_Sprite> del_col;
		texture.loadFromFile(CONST::PATH::IMAGES + "delete.png");

		for (int i = 0; page_num * page_size + i < n; ++i) {
			Textbox type(types[i].type, 18, sf::Color::Black, sf::Vector2f(50, 250 + i * 50), sf::Vector2f(1050, 50), sf::Color::White);
			type.set_outline(sf::Color::Black);

			Button_Textbox type_button(type, sf::Color::Yellow);
			access_col.add_button(type_button);

			Button_Sprite delete_button(texture, sf::Vector2f(1100, 250 + i * 50), sf::Vector2f(50, 50));
			del_col.add_button(delete_button);
		}

		Interaction interaction;
		interaction.add_button(back_button, go_back);
		interaction.add_button(previous_button, previous);
		interaction.add_button(next_button, next);
		interaction.add_input_textbox(input_add_box);
		interaction.add_button(add_button, add_type);
		
		access_col.trigger = go_to_type;
		del_col.trigger = delete_type;
		data_change = false;

        interaction.add_button_list(access_col);
        interaction.add_button_list(del_col);

		while (window.isOpen() && !app->scenes.empty() && app->scenes.back() == current_scene && !data_change) {
			window.clear(sf::Color::White);

			word.draw(window, app->scenes.default_font);
			interaction.draw(window, app->scenes.default_font);

			window.display();

			auto event = interaction.interact(window);
			app->scenes.interact(event);
		}
	}
}
