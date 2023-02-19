#pragma once

#include "Scene.h"

class Scene_Audio : public Scene
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

	Scene_Audio(GameEngine* gameEngine);

	void update() override;

	void sRender() override;
	void sDoAction(const Action& action) override;


};