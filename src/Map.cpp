#include <iostream>

#include "Map.h"
#include "RoomObject.h"
#include "WorldObject.h"
#include "DrawableObject.h"

Map::Map()
{

}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for(DrawableObject *drawable : m_drawables)
	{
		target.draw(*drawable, states);
	}
}

void Map::update(const int delta)
{
	const std::list<WorldObject *> objects = m_objects;

	for(WorldObject *object : objects)
	{
		const bool alive = object->update(delta);

		if(!alive)
		{
			delete object;
		}
	}
}

void Map::addObject(DrawableObject *worldObject)
{
	m_objects.push_back(worldObject);
	m_drawables.push_back(worldObject);
}

void Map::removeObject(DrawableObject *worldObject)
{
	m_objects.remove(worldObject);
	m_drawables.remove(worldObject);
}
