#include "Scene_Audio.h"
#include "Scene_Audio.h"
#include "Scene_Options.h"
#include "Scene_play.h"
#include <memory>

void Scene_Audio::onEnd()
{
	m_game->window().close();
}

Scene_Audio::Scene_Audio(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Audio::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "Audio";
	m_menuStrings.push_back("TODO Insert Audio Prompts");

	m_menuText.setFont(m_game->assets().getFont("ShantellSans"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);
}

void Scene_Audio::update(sf::Time dt)
{
	m_entityManager.update();
	//sRender();
}


void Scene_Audio::sRender()
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
		m_menuText.setPosition(textwidth / 10, (i + 1.f) * 96.f);
		m_menuText.setString(m_menuStrings.at(i));
		m_game->window().draw(m_menuText);
	}

	m_game->window().display();
}


void Scene_Audio::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "QUIT")
		{
			m_game->changeScene("OPTIONS", std::make_shared<Scene_Options>(m_game));
		}
	}
}