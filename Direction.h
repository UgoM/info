#include <QList>

class Direction {

	int di, dj;
	
	private:
		Direction(int i, int j);
	
	public:
	
		static const Direction NORTH_WEST;
		static const Direction NORTH_EAST;
		static const Direction SOUTH_WEST;
		static const Direction SOUTH_EAST;
		
		int i() const;
		int j() const;
		static QList<Direction> values();

};
