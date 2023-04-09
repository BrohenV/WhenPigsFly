#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include "GameEngine.h"
#include "EntityManager.h"

class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X{ 0.f }, Y{ 0.f }, CW{ 0.f }, CH{ 0.f };
		float SPEED{ 0.f }, MAXSPEED{ 0.f }, JUMP{ 0.f }, GRAVITY{ 0.f };
		std::string WEAPON;
	};

	struct ButcherConfig
	{
		float X{ 0.f }, Y{ 0.f }, CW{ 0.f }, CH{ 0.f };
		float SPEED{ 0.f }, MAXSPEED{ 0.f }, JUMP{ 0.f }, GRAVITY{ 0.f };
		std::string WEAPON;
	};

protected:

	std::shared_ptr<Entity>		m_player;
	std::shared_ptr<Entity>		m_butcher;
	std::string					m_levelPath;
	PlayerConfig				m_playerConfig;
	sf::Vector2f                m_spawnPosition;
	ButcherConfig			    m_butcherConfig;
	float                       m_butcherSpeed{ 2.f };

	bool						m_drawTextures{true};						
	bool						m_drawCollision{false}; 
	bool						m_drawGrid{false};
	const sf::Vector2f			m_gridSize{ 64,64 };
	sf::Text					m_gridText;
	void						init(const std::string& levelPath);
	void						registerActions();
	void						onEnd() override;
	float                       m_scrollSpeed{2};
	sf::Time					dt;
	void						checkPlayerState();
	sf::SoundBuffer				buffer;
	std::map<std::string, std::string>sfxMap;

public:
	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);
	
	void	update() override;
	sf::View        m_worldView;
	sf::FloatRect   m_worldBounds;
	sf::FloatRect	getViewBounds();
	void			sRender() override;
	void			sDoAction(const Action& action) override;
	void			checkIfPlayerInBounds();
	void			sMovement();
	void			sAnimation();
	void			sLifespan();
	void			sEnemySpawner();
	void			sCollision();
	void			spawnButcher(sf::Vector2f pos, std::string size);
	void            createKnife(sf::Vector2f pos);
	void			checkKnifeCollision();
	void			checkIfDead(std::shared_ptr<Entity> e);
	void			sDebug();
	void			drawLine();
	void			playerCheckState();
	sf::Vector2f	gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	void			loadLevel(const std::string& filename);
	void			loadFromFile(const std::string& filename);
	void			spawnPlayer();
	void			spawnBullet(std::shared_ptr<Entity>);
	void			removeOutOfBounds();
};




