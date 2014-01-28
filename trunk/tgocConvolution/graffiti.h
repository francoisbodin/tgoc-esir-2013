#ifndef __GRAFFITI_HEADER__
#define __GRAFFITI_HEADER__

#include <iostream>

using namespace std;

class Graffiti{
	public:
		Graffiti();
		Graffiti(string _id, string _url);
		Graffiti( const Graffiti & g);
		~Graffiti();
		// GETTERS
		string getId();
		string getImageUrl();
		string getLocalUrl();
		//SETTERS
		void setId(string _id);
		void setImageUrl(string _imageUrl);
		void setLocalUrl(string _imageUrl);
		void setLongitude(double lng);
		void setLatitude(double lat);
	        double getLongitude() const;
		double getLatitude() const;

		string toString() const;
		friend std::ostream& operator<< (std::ostream & ofs, Graffiti const &g);

//////////////FIELDS/////////////////////////
	private:
		string id;
		string imageUrl;
		string localUrl;
		double longitude;
		double latitude;
};
#endif //__GRAFFITI_HEADER__
