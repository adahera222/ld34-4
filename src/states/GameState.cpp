#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "GameState.h"
#include "InputMapping.h"
#include "Player.h"
#include "World.h"
#include "WorldPosition.h"
#include "PhysicsWorldPosition.h"
#include "PlayerInputProxy.h"
#include "MapLoader.h"
#include "RoomObject.h"
#include "WorldDebug.h"
#include "StringEx.h"
#include "CollisionListener.h"
#include "Map.h"
#include "InteractableObject.h"
#include "WorldGeneratorContext.h"
#include "WorldGenerator.h"

#include "objects/TreasureContainer.h"

#include "ai/HumanoidBot.h"
#include "ai/Pathfinder.h"

#include "ui/FloatingPanel.h"
#include "ui/HealthBar.h"

#include "equipment/LaserPistol.h"

GameState::GameState(sf::RenderWindow *window)
	: m_window(window)
	, m_lastPhysicsStepTime(0)
	, m_fpsCount(0)
{
	const b2Vec2 playerPosition(64, 64);
	const b2Vec2 treasurePosition(256, 320);

	sf::SoundBuffer *stepSound = new sf::SoundBuffer();
	stepSound->loadFromFile("resources/sound/step.wav");

	WorldDebug *worldDebugger = new WorldDebug(m_window);
	worldDebugger->SetFlags(b2Draw::e_shapeBit);

	CollisionListener *collisionListener = new CollisionListener();

	World *world = new World();
//	world->SetDebugDraw(worldDebugger);
	world->SetContactListener(collisionListener);

	b2Filter *playerCollisionFilter = new b2Filter();
	playerCollisionFilter->categoryBits = World::Player;
	playerCollisionFilter->maskBits = 0xFFFF ^ World::PlayerProjectile;

	b2Filter *playerProjectileFilter = new b2Filter();
	playerProjectileFilter->categoryBits = World::PlayerProjectile;
	playerProjectileFilter->maskBits = 0xFFFF ^ (World::Player | World::BotProjectile);

	b2Filter *botCollisionFilter = new b2Filter();
	botCollisionFilter->categoryBits = World::Bot;
	botCollisionFilter->maskBits = 0xFFFF ^ World::BotProjectile;

	b2Filter *botProjectileFilter = new b2Filter();
	botProjectileFilter->categoryBits = World::BotProjectile;
	botProjectileFilter->maskBits = 0xFFFF ^ (World::Bot | World::PlayerProjectile);

	PhysicsWorldPosition *playerWorldPosition = world->createCircle(playerPosition, 16, b2_dynamicBody, *playerCollisionFilter);
	PhysicsWorldPosition *treasureWorldPosition = world->createBox(treasurePosition, 32, 32, b2_staticBody);

	m_interactionPanel = new FloatingPanel("Press to interact", window);
	m_interactionPanel->setVisible(false);

	m_player = new Player(playerWorldPosition, m_interactionPanel, *stepSound);

	Pathfinder *pathfinder = new Pathfinder();
	MapLoader mapLoader(world, pathfinder, m_player, botCollisionFilter, botProjectileFilter, stepSound);
	WorldGenerator worldGenerator(&mapLoader, "resources/world.wld");

	m_map = worldGenerator.generate();

	LaserPistol *playerPistol = new LaserPistol(*playerWorldPosition, *playerProjectileFilter, *world, *m_map);

	m_player->setWeapon(playerPistol);

	m_proxy = new PlayerInputProxy(m_player);
	m_world = world;
	m_pathfinder = pathfinder;

	m_healthBar = new HealthBar(*m_player);

	TreasureContainer *treasureContainer = new TreasureContainer(treasureWorldPosition);
	playerWorldPosition->createRectangularSensor(16, 0, 32, 48);

	m_map->addObject(m_player);
	m_map->addObject(treasureContainer);


	// UI Initialization
	m_font.loadFromFile("resources/Oxygen-Regular.ttf");

	m_fpsText.setFont(m_font);
	m_fpsText.setCharacterSize(10);
}

void GameState::setupInput(InputMapping *mapping)
{
	AnalogMapping &analog = mapping->analog();

	KeyMapping &left = mapping->left();
	KeyMapping &right = mapping->right();
	KeyMapping &forward = mapping->forward();
	KeyMapping &backwards = mapping->backwards();
	KeyMapping &action = mapping->action();

	analog.connect(m_proxy, &PlayerInputProxy::absoluteInput, &PlayerInputProxy::relativeInput);
	left.connect(m_proxy, &PlayerInputProxy::moveLeft, &PlayerInputProxy::stopHorizontally);
	right.connect(m_proxy, &PlayerInputProxy::moveRight, &PlayerInputProxy::stopHorizontally);
	forward.connect(m_proxy, &PlayerInputProxy::moveForward, &PlayerInputProxy::stopVertically);
	backwards.connect(m_proxy, &PlayerInputProxy::moveBackwards, &PlayerInputProxy::stopVertically);
	action.connect(m_proxy, &PlayerInputProxy::action, &PlayerInputProxy::nothing);
}

void GameState::update()
{
	for(int i = 0; shouldStep(); i++)
	{
		if(!i)
		{
			const int elapsed = m_clock
				.getElapsedTime()
				.asMilliseconds();

			m_map->update(elapsed - m_lastPhysicsStepTime);
		}

		m_lastPhysicsStepTime += TIMESTEP * 1000;
		m_world->Step(TIMESTEP, 6, 2);
	}

	m_world->ClearForces();

	const int fpsTimer = m_fpsTimer
		.getElapsedTime()
		.asMilliseconds();

	if(fpsTimer > 500)
	{
		const sf::String &string = sf::StringEx::format("%1 FPS", m_fpsCount * 2);

		m_fpsText.setString(string);
		m_fpsTimer.restart();
		m_fpsCount = 0;
	}

	const b2Vec2 &playerPosition = (*m_player)
		.worldPosition()
		.position();

	sf::Listener::setPosition(playerPosition.x, playerPosition.y, 0);

	const sf::Vector2<unsigned int> &size = m_window->getSize();
	const sf::Rect<float> cameraRect(playerPosition.x - size.x / 2, playerPosition.y - size.y / 2, size.x, size.y);
	const sf::View cameraView(cameraRect);
	const sf::View &defaultView = m_window->getDefaultView();

	m_window->setView(cameraView);
	m_window->clear(sf::Color(252, 224, 180));
	m_window->draw(*m_map);

	m_world->DrawDebugData();

	m_window->setView(defaultView);
//	m_window->draw(m_fpsText);
	m_window->draw(*m_interactionPanel);
	m_window->draw(*m_healthBar);

	m_window->setView(cameraView);
	m_window->display();

	m_fpsCount++;
}

bool GameState::shouldStep()
{
	const int o = m_lastPhysicsStepTime + TIMESTEP * 1000;
	const int f = m_clock
		.getElapsedTime()
		.asMilliseconds();

	return f > o;
}
