#ifndef ROOMPLACEMENTSTRATEGY_H
#define ROOMPLACEMENTSTRATEGY_H

#include <iostream>

#include "IRoomPlacementStrategy.h"
#include "Direction.h"

template<Direction::Value TDirection>
class RoomPlacementStrategy : public IRoomPlacementStrategy
{
	public:
		Coordinate position(const Room &, const Room &) const override
		{
			return Coordinate();
		}
};

template<>
class RoomPlacementStrategy<Direction::Left> : public IRoomPlacementStrategy
{
	public:
		Coordinate position(const Room &first, const Room &second) const override
		{
			const Coordinate c1 = first.entrance(Direction::Left);
			const Coordinate c2 = second.entrance(Direction::Right);
			const Coordinate result(c1.first - second.width(), c1.second - c2.second);

			return result;
		}
};

template<>
class RoomPlacementStrategy<Direction::Right> : public IRoomPlacementStrategy
{
	public:
		Coordinate position(const Room &first, const Room &second) const override
		{
			const Coordinate c1 = first.entrance(Direction::Right);
			const Coordinate c2 = second.entrance(Direction::Left);
			const Coordinate result(c1.first, c1.second - c2.second);

			return result;
		}
};

template<>
class RoomPlacementStrategy<Direction::Top> : public IRoomPlacementStrategy
{
	public:
		Coordinate position(const Room &first, const Room &second) const override
		{
			std::cout << "TOP" << std::endl;

			const Coordinate c1 = first.entrance(Direction::Top);
			const Coordinate c2 = second.entrance(Direction::Bottom);
			const Coordinate result(c1.first - c2.first, -second.height());

			return result;
		}
};

template<>
class RoomPlacementStrategy<Direction::Bottom> : public IRoomPlacementStrategy
{
	public:
		Coordinate position(const Room &first, const Room &second) const override
		{
			std::cout << "BOTTOM" << std::endl;

			const Coordinate c1 = first.entrance(Direction::Bottom);
			const Coordinate c2 = second.entrance(Direction::Top);
			const Coordinate result(c1.first - c2.first, c1.second);

			return result;
		}
};

#endif // ROOMPLACEMENTSTRATEGY_H
