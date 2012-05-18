#include "src/games/checkers/Direction.h"

const Direction Direction::NORTH_WEST = Direction(-1, -1);
const Direction Direction::NORTH_EAST = Direction(-1, 1);
const Direction Direction::SOUTH_WEST = Direction(1, -1);
const Direction Direction::SOUTH_EAST = Direction(1, 1);
const Direction Direction::UNDEFINED = Direction(0, 0);

Direction::Direction(int ii, int jj) {
	di = ii;
	dj = jj;
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

QList<Direction> Direction::getOrthogonalDirections(Direction dir) {
	QList<Direction> dirList;
	if (dir == NORTH_WEST) {
		dirList << NORTH_EAST << SOUTH_WEST;
	} else if (dir == NORTH_EAST) {
		dirList << SOUTH_EAST << NORTH_WEST;
	} else if (dir == SOUTH_EAST) {
		dirList << SOUTH_WEST << NORTH_EAST;
	} else if (dir == SOUTH_WEST) {
		dirList << NORTH_WEST << SOUTH_EAST;
	}
	return dirList;
}
