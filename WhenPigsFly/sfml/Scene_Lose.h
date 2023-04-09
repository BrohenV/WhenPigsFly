#pragma once

#include "Scene.h"

class Scene_Lose : public Scene
{
private:
	std::vector<std::string>	m_menuStrings;
	sf::Text					m_menuText;
	std::vector<std::string>	m_levelPaths;
	int							m_menuIndex{ 0 };
	std::string					m_title;


	void init();
	void onEnd() override;
public:
	Scene_Lose(GameEngine* gameEngine);

	void update() override;


	void sRender() override;
	void sDoAction(const Action& action) override;
};

