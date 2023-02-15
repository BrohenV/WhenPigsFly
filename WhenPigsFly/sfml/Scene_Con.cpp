#include "Scene_Con.h"
#include "Scene_Con.h"
#include "Scene_Options.h"
#include "Scene_play.h"
#include <memory>

void Scene_Con::onEnd()
{
	m_game->window().close();
}

Scene_Con::Scene_Con(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Con::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "Menu & Game Controls";
	m_menuStrings.push_back("WASD or Arrow Keys = Flight / Selection");
	m_menuStrings.push_back("T = Toggle Textures");
	m_menuStrings.push_back("C = Toggle Collisions");
	m_menuStrings.push_back("G = Toggle Grid");
	m_menuStrings.push_back("P = Pause");
	m_menuStrings.push_back("Esc = Go back or Quit");

	m_menuText.setFont(m_game->assets().getFont("Arial"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);
}

void Scene_Con::update()
{
	m_entityManager.update();
	//sRender();
}


void Scene_Con::sRender()
{

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);
	m_game->window().setView(view);

	static const sf::Color selectedColor(0, 0, 0);
	static const sf::Color normalColor(0, 0, 0);
	static const sf::Color backgroundColor(100, 100, 255);

	m_game->window().clear(backgroundColor);

	m_menuText.setFillColor(normalColor);
	m_menuText.setString(m_title);
	float textwidth = m_menuText.getGlobalBounds().width;
	m_menuText.setPosition(m_game->window().getSize().x / 2.f - textwidth / 2, 10);
	m_game->window().draw(m_menuText);

	for (size_t i{ 0 }; i < m_menuStrings.size(); ++i)
	{
		m_menuText.setFillColor((i == m_menuIndex ? selectedColor : normalColor));
		m_menuText.setPosition(textwidth / 10, (i + 1) * 96);
		m_menuText.setString(m_menuStrings.at(i));
		m_game->window().draw(m_menuText);
	}

	m_game->window().display();
}


void Scene_Con::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "QUIT")
		{
			m_game->changeScene("OPTIONS", std::make_shared<Scene_Options>(m_game));
		}
	}
}