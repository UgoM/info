#ifndef DEF_PICTURE
#define DEF_PICTURE

#include <QPixmap>

class Picture {

	QPixmap picture;
	
	protected:
		Picture() {}
	
	public:
		const QPixmap & getPicture() const {
			return picture;
		}
		void setPicture(const QPixmap & picture) {
			this->picture = picture;
		}

};

#endif