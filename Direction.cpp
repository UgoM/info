#include "Direction.h"

Direction::Direction(int i, int j) {
	di = i;
	dj = j;
}

int Direction::i() const {
	return di;
}

int Direction::j() const {
	return dj;
}

QList<Direction> Direction::values() {
	QList<Direction> dirs;
	dirs << NORTH_WEST << NORTH_EAST << SOUTH_WEST << SOUTH_EAST;
	return dirs;
}

const Direction Direction::NORTH_WEST = Direction(-1, -1);
const Direction Direction::NORTH_EAST = Direction(-1, 1);
const Direction Direction::SOUTH_WEST = Direction(1, -1);
const Direction Direction::SOUTH_EAST = Direction(1, 1);