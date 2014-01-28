#include "apirequester.h"


void ApiRequester::getAllGraffitis(std::vector<Graffiti> *list, int maxe = 10000){
	int start=1;
	int step = 4;
	bool run = true;
	std::stringstream response;
	while(run){
		getChunk(start, step, response);
		run = parseResponse(response, list)==step;
		start+= step;
		if (start >= maxe) break;
	}
}

void ApiRequester::saveImageFromServer(Graffiti *g, const std::string location){
  	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl;
	FILE *fp;
	CURLcode res;
	stringstream outfilename;
	outfilename<<location<<"/"<<g->getImageUrl();
	stringstream url;
	url<<URL_STORAGE<<g->getImageUrl();

	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename.str().c_str(),"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		std::cout<<outfilename.str()<<std::endl;
		g->setLocalUrl(outfilename.str());
	}
}

/////////////////PRIVATE


size_t ApiRequester::write_file_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written;
  if (!stream) return 0;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

size_t ApiRequester::write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	std::string buf = std::string(static_cast<char *>(ptr), size * nmemb);
  	std::stringstream *response = static_cast<std::stringstream *>(stream);
  	response->write(buf.c_str(), (std::streamsize)buf.size());
  	return size * nmemb;
}

bool ApiRequester::getChunk(int start, int limit, std::stringstream &response){
	curl_global_init(CURL_GLOBAL_ALL);
	curl_slist *headerlist = NULL;
	stringstream url;
	url<<URL_API<<"graffiti/chunk/"<<start<<"?limit="<<limit;
	std::cout<<"Get : "<<url.str()<<std::endl;
	CURL *curl = curl_easy_init();
	char ebuf[CURL_ERROR_SIZE];
	curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, ebuf);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	CURLcode res = curl_easy_perform(curl); 
	curl_easy_cleanup(curl);

	return res == CURLE_OK;
}

int ApiRequester::parseResponse(std::stringstream &stream, std::vector<Graffiti> *list){
	int entry =0;
	pugi::xml_document doc;
	pugi::xml_parse_result result =  doc.load(stream);
	pugi::xml_node graffitis = doc.child("discovart_model-graffitis");
	for (pugi::xml_node graffiti = graffitis.first_child(); graffiti; graffiti = graffiti.next_sibling()){
    	entry++;
        std::string id = graffiti.child("id").text().get();
        std::string url = graffiti.child("imageURL").text().get();
	std::string lng = graffiti.child("longitude").text().get();
	std::string lat = graffiti.child("latitude").text().get();
	std::cout << "graffiti (ID :"<< id << ",   ImageUrl: " << url << " lng: " << lng 
		  << " lat: " << lat << ")" << std::endl;
        Graffiti *g = new Graffiti(id, url); 
	g->setLongitude(atof(lng.c_str()));
	g->setLatitude(atof(lat.c_str()));
        list->push_back(*g);

    }
    return entry;
}




