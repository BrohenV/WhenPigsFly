#include "Scene_Menu.h"
#include "Scene_Options.h"
#include "Scene_play.h"
#include <memory>

void Scene_Menu::onEnd()
{
	m_game->window().close();
}

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}



void Scene_Menu::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "When Pigs Fly";
	m_menuStrings.push_back("New Game");
	m_menuStrings.push_back("Options");
	m_menuStrings.push_back("Quit");

	m_levelPaths.push_back("../assets/level1.txt");
	m_levelPaths.push_back("QUIT");

	m_menuText.setFont(m_game->assets().getFont("Arial"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Menu::update()
{
	m_entityManager.update();
	//sRender();
}


void Scene_Menu::sRender()
{

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	static const sf::Color selectedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);

	static const sf::Color backgroundColor(100, 100, 255);

	sf::Text footer("Esc to go back / Arrow keys to navigate",
		m_game->assets().getFont("Arial"), 20);
	footer.setFillColor(normalColor);
	footer.setPosition(32, 700);

	m_game->window().clear(backgroundColor);

	m_menuText.setFillColor(normalColor);
	m_menuText.setString(m_title);
	float textwidth = m_menuText.getGlobalBounds().width;
	m_menuText.setPosition(m_game->window().getSize().x / 2.f - textwidth / 2, 10);
	m_game->window().draw(m_menuText);

	for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
	{
		m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
		m_menuText.setPosition(textwidth / 2, 32 + (i + 1) * 96);
		m_menuText.setString(m_menuStrings.at(i));
		m_game->window().draw(m_menuText);
	}

	m_game->window().draw(footer);
	m_game->window().display();

}


void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			m_menuIndex = (m_menuIndex + m_menuStrings.size() - 1) % m_menuStrings.size();
		}
		else if (action.name() == "DOWN")
		{
			m_menuIndex = (m_menuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			switch (m_menuIndex) {
				case 0:
					m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_menuIndex]));
					break;
				case 1:
					m_game->changeScene("OPTIONS", std::make_shared<Scene_Options>(m_game));
					break;
				default:
					//error
					std::cout << "Error";
			}
			
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}

}
