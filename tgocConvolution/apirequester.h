#ifndef __API_REQUEST_H__
#define  __API_REQUEST_H__
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "graffiti.h"
#include "libs/pugixml/pugixml.hpp"


//TODO : rename this class :)


#define URL_API "http://discovart.herokuapp.com/"
#define URL_STORAGE "http://discovart.irisa.fr/database/"


class ApiRequester{
	
	public:
                static void getAllGraffitis(std::vector<Graffiti> *list, int maxe);
		static void saveImageFromServer(Graffiti *g, const std::string location);


	private:
		static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
		static size_t write_file_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
		static bool getChunk(int start, int limit, std::stringstream &response);
		static int parseResponse(std::stringstream &stream, std::vector<Graffiti> *list);
};
#endif  //__API_REQUEST_H__
