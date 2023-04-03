
#include "Scene_play.h"
#include "Entity.h"
#include "Components.h"
#include "Physics.h"
#include <string>
#include "Utilities.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include "Scene_Menu.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
	
}

void Scene_Play::init(const std::string& levelPath)
{
	registerActions();

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assets().getFont("ShantellSans"));

	loadLevel(levelPath);
	//MusicPlayer::getInstance().play("loseTheme");
}

void Scene_Play::registerActions()
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");

	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");

	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::Left, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::Right, "RIGHT");

	registerAction(sf::Keyboard::W, "JUMP");
	registerAction(sf::Keyboard::Up, "JUMP");

	registerAction(sf::Keyboard::Space, "SHOOT");
}

void Scene_Play::update()
{
	m_entityManager.update();
	

	// TODO pause function
	if (!m_isPaused) {
		sf::View view = m_game->window().getView();
		//std::cout << view.getCenter().x;
		view.setCenter(view.getCenter().x + m_scrollSpeed, m_game->window().getSize().y - view.getCenter().y);
		m_game->window().setView(view);
		sMovement();
		sLifespan();
		sCollision();
		sAnimation();
		playerCheckState();
		killOutOfBounds();
		
	}
}

void Scene_Play::checkPlayerState() {
}

sf::FloatRect Scene_Play::getViewBounds() {
	sf::FloatRect bounds;
	bounds.left = m_worldView.getCenter().x - m_worldView.getSize().x / 2.f;
	bounds.top = m_worldView.getCenter().y - m_worldView.getSize().y / 2.f;
	bounds.width = m_worldView.getSize().x;
	bounds.height = m_worldView.getSize().y;
	return bounds;
}

void Scene_Play::sMovement()
{

	if (m_player->isActive()) {
		// player movement 
		auto& pt = m_player->getComponent<CTransform>();
		pt.vel.x = 0.f;

		if (m_player->getComponent<CInput>().left)
			pt.vel.x -= 1;

		if (m_player->getComponent<CInput>().right)
			pt.vel.x += 1;

		if (m_player->getComponent<CInput>().up){
			m_player->getComponent<CInput>().up = false;
			pt.vel.y = -m_playerConfig.JUMP + 10;
		}

		pt.vel.x = pt.vel.x * m_playerConfig.SPEED;
		// facing direction
		if (pt.vel.x < -0.1)
			m_player->getComponent<CState>().set(CState::isFacingLeft);
		if (pt.vel.x > 0.1)
			m_player->getComponent<CState>().unSet(CState::isFacingLeft);

	}
	// gravity
	for (auto e : m_entityManager.getEntities())
	{
		if ((e->getTag() != "player") ||
			((e->getTag() == "player") && e->isActive())) {
			if (e->hasComponent<CGravity>()) {
				auto& tx = e->getComponent<CTransform>();
				tx.vel.y += e->getComponent<CGravity>().g;
			}	
		}

		if (e->getTag() == "butcher") {
			auto& pt = e->getComponent<CTransform>();
			pt.pos += pt.vel * dt.asSeconds();
			//pt.rot += pt.rotVel * dt.asSeconds();
		}
	}

	// move all entities
	for (auto e : m_entityManager.getEntities())
	{
		auto& tx = e->getComponent<CTransform>();
		tx.prevPos = tx.pos;
		tx.pos += tx.vel;
	}
}

void Scene_Play::playerCheckState()
{
	auto& tx = m_player->getComponent<CTransform>();
	auto& state = m_player->getComponent<CState>();

	// face the right way
	if (std::abs(tx.vel.x) > 0.1f)
		tx.scale.x = (tx.vel.x > 0.f) ? 1.f : -1.f;

	if (!state.test(CState::isGrounded))
	{
		m_player->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Air");
	}
	else
	{
		// if grounded  
		if (std::abs(tx.vel.x) > 0.1f)
		{
			if (!state.test(CState::isRunning)) // wasn't running
			{
				// change to running animation
				m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
				state.set(CState::isRunning);
			}
		}
		else
		{
			m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
			state.unSet(CState::isRunning);
		}
	}
}

void Scene_Play::sLifespan()
{
	// move all entities
	for (auto e : m_entityManager.getEntities("bullet"))
	{
		auto& lifespan = e->getComponent<CLifespan>();
		if (lifespan.has)
		{
			lifespan.remaining -= 1;
			if (lifespan.remaining < 0)
			{
				e->addComponent<CAnimation>(m_game->assets().getAnimation("Explosion"), false);
				e->getComponent<CLifespan>().has = false;
				e->getComponent<CTransform>().vel.x *= 0.1f;
			}
		}
	}
}

void Scene_Play::sEnemySpawner()
{
}

void Scene_Play::sCollision()
{
	// player with tile
	auto players = m_entityManager.getEntities("player");
	auto tiles = m_entityManager.getEntities("tile");

	for (auto p : players)
	{
		p->getComponent<CState>().unSet(CState::isGrounded); // not grounded 
		for (auto t : tiles)
		{
			auto overlap = Physics::getOverlap(p, t);
			if (overlap.x > 0 && overlap.y > 0)  // +ve overlap in both x and y means collision
			{
				auto prevOverlap = Physics::getPreviousOverlap(p, t);
				auto& ptx = p->getComponent<CTransform>();
				auto ttx = t->getComponent<CTransform>();

				// collision is in the y direction
				if (prevOverlap.x > 0)
				{
					if (ptx.prevPos.y < ttx.prevPos.y)
					{   // player standing on something isGrounded
						p->getComponent<CTransform>().pos.y -= overlap.y;
						p->getComponent<CInput>().canJump = true;
						p->getComponent<CState>().set(CState::isGrounded);
					}
					else
					{  // player hit something from below
						p->getComponent<CTransform>().pos.y += overlap.y;
					}
					p->getComponent<CTransform>().vel.y = 0.f;
				}

				// collision is in the x direction
				if (prevOverlap.y > 0)
				{
					if (ptx.prevPos.x < ttx.prevPos.x) // player left of tile
						p->getComponent<CTransform>().pos.x -= overlap.x;
					else
						p->getComponent<CTransform>().pos.x += overlap.x;
				}
			}
		}
	}
}

void Scene_Play::sRender()
{
	// background changes if paused
	static const sf::Color background(100, 100, 255);
	static const sf::Color pauseBackground(50, 50, 150);
	m_game->window().clear((m_isPaused ? pauseBackground : background));

	// set the view to center on the player 
	// this is a side scroller so only worry about X axis
	auto& pPos = m_player->getComponent<CTransform>().pos;
	float centerX = std::max(m_game->window().getSize().x / 2.f, pPos.x);
	sf::View view = m_game->window().getView();
	

	// draw all entities
	if (m_drawTextures)
	{
		for (auto e : m_entityManager.getEntities())
		{

			if (e->hasComponent<CAnimation>())
			{
				auto& transform = e->getComponent<CTransform>();

				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	// draw all collisioni bound boxes
	if (m_drawCollision)
	{
		for (auto e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x, box.size.y));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.pos.x, transform.pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(0, 255, 0));
				rect.setOutlineThickness(1.f);
				m_game->window().draw(rect);
			}
		}
	}

	// draw grid
	sf::VertexArray lines(sf::Lines);
	sf::Text gridText;
	gridText.setFont(m_game->assets().getFont("ShantellSans"));
	gridText.setCharacterSize(10);

	if (m_drawGrid)
	{
		float left = view.getCenter().x - view.getSize().x / 2.f;
		float right = left + view.getSize().x;
		float top = view.getCenter().y - view.getSize().y / 2.f;
		float bot = top + view.getSize().y;

		// aling grid to grid size
		int nCols = static_cast<int>(view.getSize().x / m_gridSize.x);
		int nRows = static_cast<int>(view.getSize().y / m_gridSize.y);


		// row and col # of bot left
		const int ROW0 = 768;
		int firstCol = static_cast<int>(left) / static_cast<int>(m_gridSize.x);
		int lastRow = static_cast<int>(bot) / static_cast<int>(m_gridSize.y);

		lines.clear();

		// vertical lines
		for (int x{ 0 }; x <= nCols; ++x)
		{
			lines.append(sf::Vector2f((firstCol + x) * m_gridSize.x, top));
			lines.append(sf::Vector2f((firstCol + x) * m_gridSize.x, bot));
		}

		// horizontal lines
		for (int y{ 0 }; y <= nRows; ++y)
		{
			lines.append(sf::Vertex(sf::Vector2f(left, ROW0 - (lastRow - y) * m_gridSize.y)));
			lines.append(sf::Vertex(sf::Vector2f(right, ROW0 - (lastRow - y) * m_gridSize.y)));
		}

		// grid coordinates
		// (firstCol, lastRow) is the bottom left corner of the view 
		for (int x{ 0 }; x <= nCols; ++x)
		{
			for (int y{ 0 }; y <= nRows; ++y)
			{
				std::string label = std::string("(" + std::to_string(firstCol + x) + ", " + std::to_string(lastRow - y) + ")");
				gridText.setString(label);
				gridText.setPosition((x + firstCol) * m_gridSize.x, ROW0 - (lastRow - y + 1) * m_gridSize.y);
				m_game->window().draw(gridText);
			}
		}

		m_game->window().draw(lines);
	}

	if (m_isPaused) {
		sf::Text paused("PAUSED", m_game->assets().getFont("ShantellSans"), 128);
		centerOrigin(paused);
		auto center = m_game->window().getView().getCenter();
		// FIX PAUSED POSITION
		paused.setPosition(center);
		m_game->window().draw(paused);
	}

	m_game->window().display();
}

void Scene_Play::sDoAction(const Action& action)
{
	// On Key Press
	if (action.type() == "START")
	{
		sf::Vector2f pos = sf::Vector2f(
			m_player->getComponent<CTransform>().pos.x,
			m_player->getComponent<CTransform>().pos.y);

		if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
		else if (action.name() == "PAUSE") { 
			setPaused(!m_isPaused);
			

		}
		else if (action.name() == "QUIT") { onEnd(); }

		// Player control
		else if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = true; }
		else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = true; }

		else if (action.name() == "JUMP") {
			if (m_player->getComponent<CInput>().canJump && m_player->getComponent<CState>().isDead != true) {
				m_player->getComponent<CInput>().up = true;
				SoundPlayer::getInstance().play("WingsUp", pos, 25);
			}
		}

		else if (action.name() == "SHOOT") {
			if (m_player->getComponent<CInput>().canShoot)
			{
				spawnBullet(m_player);
				m_player->getComponent<CInput>().shoot = true;
				m_player->getComponent<CInput>().canShoot = false;
				//SoundPlayer::getInstance().play("BaconBomb", pos, 25);

				
			}
		}

		//MusicPlayer::getInstance().play("winTheme");
	}

	// on Key Release 
	else if (action.type() == "END")
	{
		if (action.name() == "LEFT") { m_player->getComponent<CInput>().left = false; }
		else if (action.name() == "RIGHT") { m_player->getComponent<CInput>().right = false; }
		else if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = false; }
		else if (action.name() == "SHOOT") { m_player->getComponent<CInput>().canShoot = true; }
	}
}

void Scene_Play::sAnimation()
{

	// m_player->getComponent<CAnimation>().animation.update();

	for (auto e : m_entityManager.getEntities())
	{
		auto& anim = e->getComponent<CAnimation>();
		if (anim.has)
		{
			anim.animation.update(anim.repeat);
			if (anim.animation.hasEnded())
				e->destroy();
		}
	}
}

void Scene_Play::onEnd()
{
	m_game->changeScene("MENU", nullptr, true);
}

void Scene_Play::drawLine()
{
}

void Scene_Play::sDebug()
{
}

sf::Vector2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	// (left, bot) of grix,gidy)

	// this is for side scroll, and based on window height being the same as world height
	// to be more generic and support scrolling up and down as well as left and right it
	// should be based on world size not window size
	float x = 0.f + gridX * m_gridSize.x;
	float y = 768.f - gridY * m_gridSize.y;

	sf::Vector2f spriteSize = entity->getComponent<CAnimation>().animation.getSize();

	return sf::Vector2f(x + spriteSize.x / 2.f, y - spriteSize.y / 2.f);
}

void Scene_Play::loadLevel(const std::string& path)
{
	m_entityManager = EntityManager(); // get a new entity manager

	loadFromFile(path); // TODO read in level file 
	spawnPlayer();
	


}

void Scene_Play::loadFromFile(const std::string& path)
{
	// Read Config file 
	std::ifstream confFile(path);
	if (confFile.fail())
	{
		std::cerr << "Open file: " << path << " failed\n";
		confFile.close();
		exit(1);
	}

	std::string token{ "" };
	confFile >> token;
	while (confFile)
	{
		confFile >> token;
		if (token == "Tile")
		{
			std::string name;
			float gx, gy;
			confFile >> name >> gx >> gy;

			auto e = m_entityManager.addEntity("tile");
			e->addComponent<CAnimation>(m_game->assets().getAnimation(name), true);
			e->addComponent<CBoundingBox>(m_game->assets().getAnimation(name).getSize());
			e->addComponent<CTransform>(gridToMidPixel(gx, gy, e));
		}
		else if (token == "Dec")
		{
			std::string name;
			float gx, gy;
			confFile >> name >> gx >> gy;

			auto e = m_entityManager.addEntity("dec");
			e->addComponent<CAnimation>(m_game->assets().getAnimation(name), true);
			e->addComponent<CTransform>(gridToMidPixel(gx, gy, e));
		}
		else if (token == "Player")
		{
			confFile >>
				m_playerConfig.X >>
				m_playerConfig.Y >>
				m_playerConfig.CW >>
				m_playerConfig.CH >>
				m_playerConfig.SPEED >>
				m_playerConfig.JUMP >>
				m_playerConfig.MAXSPEED >>
				m_playerConfig.GRAVITY >>
				m_playerConfig.WEAPON;
		}
		else if (token == "Butcher")
		{
			sf::Vector2f pos;
			confFile >> pos.x >> pos.y;
			spawnButcher(pos);
		}
		else if (token == "Sound") {
		
			std::string key, path;
			confFile >> key >> path;
			std::cout << key;
			//sfxMap[key] = path;
	
			SoundPlayer::getInstance().loadBuffer(key, path);
			std::cout << "(" << key << "," << path << ")";
		}
		else if (token == "#")
		{
			; // ignore comments
			std::string tmp;
			std::getline(confFile, tmp);
			std::cout << "# " << tmp << "\n";
		}
		else
		{
			std::cerr << "Unkown asset type: " << token << std::endl;
		}
	}
}

void Scene_Play::spawnPlayer()
{

	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
	m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player));
	m_player->addComponent<CBoundingBox>(sf::Vector2f(m_playerConfig.CW, m_playerConfig.CH));
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<CState>();

}

void Scene_Play::spawnButcher(sf::Vector2f pos) {

	auto vel = sf::Vector2f(m_butcherSpeed, 0.f);
	float rotation = 180.f;

	auto butcher = m_entityManager.addEntity("butcher");
	butcher->addComponent<CTransform>(pos, vel, rotation);
	butcher->addComponent<CAnimation>(m_game->assets().getAnimation("Buster"), true);//TODO: Add Run
	butcher->addComponent<CCollision>(20.f);
	//butcher->addComponent<CKnife>();
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> e)
{
	auto tx = e->getComponent<CTransform>();
	sf::Vector2f pos = sf::Vector2f(
			m_player->getComponent<CTransform>().pos.x,
			m_player->getComponent<CTransform>().pos.y);

	if (tx.has)
	{
		auto bullet = m_entityManager.addEntity("bullet");
		bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.WEAPON), true);
		bullet->addComponent<CTransform>(tx.pos);
		bullet->addComponent<CBoundingBox>(m_game->assets().getAnimation(m_playerConfig.WEAPON).getSize());
		bullet->addComponent<CLifespan>(50);
		bullet->getComponent<CTransform>().vel.x = 10.f * (e->getComponent<CState>().test(CState::isFacingLeft) ? -1 : 1);
		bullet->getComponent<CTransform>().vel.y = 0.f;
		

	}
}


void Scene_Play::checkIfPlayerInBounds() {
}

void Scene_Play::killOutOfBounds()
{
	float killDepth = 710;
	auto plrPos = m_player->getComponent<CTransform>().pos;
	auto view = m_game->window().getView();

	if (plrPos.x < (view.getCenter().x - (view.getSize().x / 2)) ||
		plrPos.y < getViewBounds().top ||
		plrPos.y > killDepth
		) {
		m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
		//SoundPlayer::getInstance().removeStoppedSounds();
		//m_player->destroy();
		//spawnPlayer();
		//m_player = nullptr;
		//m_game->changeScene<C
		MusicPlayer::getInstance().play("loseTheme");

	}

	
}


