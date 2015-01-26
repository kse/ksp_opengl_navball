#include <jsoncpp/json/json.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "Telemachus.hpp"

static size_t
curlMemoryWriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct Storage *mem = (struct Storage *)userp;

	if (mem->length + realsize + 1 > mem->alloced) {
		mem->string = (char *)realloc(mem->string, mem->length + realsize + 1);
		if(mem->string == NULL) {
			/* out of string! */ 
			printf("not enough mem (realloc returned NULL)\n");
			return 0;
		}
		mem->alloced = mem->length + realsize + 1;
	}

	memcpy(&(mem->string[mem->length]), contents, realsize);
	mem->length += realsize;
	mem->string[mem->length] = 0;

	return realsize;
}

bool Telemachus::getPitchHeadingRoll(double *pitch, double *heading, double *roll) {
	CURLcode res;

	curl_easy_setopt(curl, CURLOPT_URL,
			"http://127.0.0.1:8085/telemachus/datalink?h=n.rawheading&p=n.rawpitch&r=n.rawroll");
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cout << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
		//exit(EXIT_FAILURE);
		return false;
	}

	//for (unsigned long i = 0; i < storage.length; i++) {
	//	putchar(storage.string[i]);
	//}
	//putchar('\n');

	Json::Value root;
	Json::Reader reader;
	bool success = reader.parse(storage.string, root);

	if (!success) {
		std::cout << reader.getFormattedErrorMessages();
		return false;
	}

	*pitch = root.get("p", Json::Value(0.0)).asDouble();
	*heading = root.get("h", Json::Value(0.0)).asDouble();
	*roll = root.get("r", Json::Value(0.0)).asDouble();

	storage.length = 0;
	return true;
}

Telemachus::Telemachus() {
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	storage.length = 0;
	storage.alloced = 1;
	storage.string = (char *)malloc(1);

	if (!curl) {
		std::cout << "Error creating curl object" << std::endl;
		exit(EXIT_FAILURE);
	}

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlMemoryWriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&storage);
}

Telemachus::~Telemachus() {
	free(storage.string);
	curl_easy_cleanup(curl);
}
