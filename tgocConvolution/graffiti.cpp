#include "graffiti.h"

Graffiti::Graffiti(){
	id = "";
	imageUrl = "";
	localUrl ="";
	longitude = 0.0;
	latitude = 0.0;
}
Graffiti::Graffiti(string _id, string _url){
	id = _id;
	imageUrl = _url;
	localUrl ="";
	longitude = 0.0;
	latitude = 0.0;
}

Graffiti::Graffiti( const Graffiti & g){
	id=g.id;
	imageUrl = g.imageUrl;
	localUrl = g.localUrl;
	longitude = g.longitude;
	latitude = g.latitude;
}

Graffiti::~Graffiti(){
}

string Graffiti::getId(){
	return id;
}

string Graffiti::getImageUrl(){
	return imageUrl;
}

string Graffiti::getLocalUrl(){
	return localUrl;
}

void Graffiti::setId(string _id){
	id = _id;
}

void Graffiti::setImageUrl(string _imageUrl){
	imageUrl = _imageUrl;
}

void Graffiti::setLocalUrl(string _localUrl){
	localUrl = _localUrl;
}


string Graffiti::toString() const{
  char buff[1000];
  sprintf(buff,"%f",getLongitude());
  string vlo=  (string) buff;
  sprintf(buff,"%f",getLatitude());
  string vla= (string) buff;
  return "(id: "+id+", url: "+imageUrl+ " lng: " + vlo + " lat: " + vla + ")";
}

std::ostream& operator<<(std::ostream & ofs, Graffiti const & g)
{
    ofs<<g.toString();
    return ofs;
}


void Graffiti::setLongitude(double lng){
  longitude = lng;
}

void Graffiti::setLatitude(double lat){
  latitude = lat;
}

double Graffiti::getLongitude() const{
  return longitude;
}

double Graffiti::getLatitude()const {
  return latitude;
}

