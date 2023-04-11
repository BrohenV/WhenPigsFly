#include "Scene_Options.h"
#include "Scene_Options.h"
#include "Scene_play.h"
#include <memory>
#include "Scene_Menu.h"
#include "Scene_Con.h"
#include "Scene_Sum.h"
#include "Scene_Credits.h"
#include "Scene_Audio.h"

void Scene_Options::onEnd()
{
	m_game->window().close();
}

Scene_Options::Scene_Options(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Options::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "Options";
	m_menuStrings.push_back("Controls");
	m_menuStrings.push_back("Summary");
	m_menuStrings.push_back("Credits");
	//m_menuStrings.push_back("Audio");

	m_menuText.setFont(m_game->assets().getFont("ShantellSans"));

	const size_t CHAR_SIZE{ 64 };
	m_menuText.setCharacterSize(CHAR_SIZE);

}

void Scene_Options::update(sf::Time dt)
{
	m_entityManager.update();
	//sRender();
}


void Scene_Options::sRender()
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
	bkg.setTexture(m_game->assets().getTexture("menusBkg"));

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

	sf::Text footer("Esc to go back / Arrow keys to navigate",
		m_game->assets().getFont("ShantellSans"), 20);
	footer.setFillColor(normalColor);
	footer.setPosition(32, 700);
	m_game->window().draw(footer);
	m_game->window().display();

}


void Scene_Options::sDoAction(const Action& action)
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
				m_game->changeScene("CONTROLS", std::make_shared<Scene_Con>(m_game));
				break;
			case 1:
				m_game->changeScene("SUMMARY", std::make_shared<Scene_Sum>(m_game));
				break;
			case 2:
				m_game->changeScene("CREDITS", std::make_shared<Scene_Credits>(m_game));
				break;
			//case 3:
			//	m_game->changeScene("AUDIO", std::make_shared<Scene_Audio>(m_game));
			//	break;
			default:
				//error
				std::cout << "Error";
			}
			
		}
		else if (action.name() == "QUIT")
		{
			m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
		}
	}

}
