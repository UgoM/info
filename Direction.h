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
		static const Direction UNDEFINED;
		
		int i() const;
		int j() const;
		static QList<Direction> getOrthogonalDirections(Direction dir);
		static QList<Direction> values();
		
		bool operator==(const Direction &other) const {
			return this->i() == other.i() && this->j() == other.j();
		}

};
