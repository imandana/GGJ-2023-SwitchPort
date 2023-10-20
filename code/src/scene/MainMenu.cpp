#include "scene/MainMenu.h"

MainMenu::MainMenu() :
	Scene()
{
	font.loadFromFile("../resources/font/arial.ttf");

	main_menu_ui = std::make_shared<MainMenuController>(font);

	auto& ui = ServiceLocator::getService<UI::UiManager>();
	ui.push(main_menu_ui);

	if (!bg_texture.loadFromFile("../resources/title.jpg"))
	{
		std::cout << "Error load texture main menu background!" << std::endl;
	}

	bg_sprite.setTexture(bg_texture);

	auto& audio = ServiceLocator::getService<AudioManager>();
	audio.addBgm("main_bgm", "../resources/Audio/Big Day Out.ogg");
	audio.playBgm("main_bgm", true, 50.f);

	buttonSetup();

	auto& input = ServiceLocator::getService<InputManager>();
	main_menu_ui->onClick();
}

MainMenu::~MainMenu()
{
	std::cout << "main menu destroyed" << std::endl;
	main_menu_ui.reset();
}

void MainMenu::update(float dt)
{

}

void MainMenu::draw(sf::RenderWindow& window)
{
	window.draw(bg_sprite);
	main_menu_ui->draw(window);
}

void MainMenu::buttonSetup()
{
	main_menu_ui->mm_view.new_game_button.setOnClickCb([this]
		{
			overworld = std::make_shared<Overworld>();
			ServiceLocator::getService<SceneManager>().load_scene(std::dynamic_pointer_cast<Scene>(overworld));
			ServiceLocator::getService<UI::UiManager>().remove(main_menu_ui);
		});

	main_menu_ui->mm_view.load_game_button.setOnClickCb([this] {
		std::cout << "Custom On Load Game Clicked" << std::endl;
		});

	main_menu_ui->mm_view.settings_button.setOnClickCb([this] {
		std::cout << "Custom On Settings Clicked" << std::endl;
		});

	main_menu_ui->mm_view.quit_button.setOnClickCb([this] {
		ServiceLocator::getService<GameEventManager>().quitGame();
		});

}