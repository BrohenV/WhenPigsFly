#include "Scene_Win.h"
#include <memory>
#include "Scene_Menu.h"
#include "MusicPlayer.h"
#include "Scene_play.h"

void Scene_Win::onEnd()
{
	m_game->window().close();
}

Scene_Win::Scene_Win(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
	MusicPlayer::getInstance().play("winTheme");
}

void Scene_Win::init()
{
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "YOU WIN";
	m_menuStrings.push_back("        Press Esc to return to menu");
	m_levelPaths.push_back("QUIT");

	m_menuText.setFont(m_game->assets().getFont("ShantellSans"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Win::update()
{
	m_entityManager.update();
	//sRender();
}

void Scene_Win::sRender()
{

	static const sf::Color selectedColor(255, 255, 255);
	static const sf::Color normalColor(0, 0, 0);
	static const sf::Color backgroundColor(100, 100, 255);
	m_game->window().clear(backgroundColor);

	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.f, m_game->window().getSize().y / 2.f);

	auto windowSize = m_game->window().getSize();
	m_game->window().setView(view);

	sf::Sprite bkg = sf::Sprite{};
	bkg.setTexture(m_game->assets().getTexture("winBkg"));

	auto bkgPosY = bkg.getLocalBounds().height - windowSize.y;
	bkg.setOrigin(0.f, bkgPosY);
	m_game->window().draw(bkg);


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

void Scene_Win::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
	    if (action.name() == "QUIT")
		{
			m_game->quitLevel();
			m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
		}
	}

}