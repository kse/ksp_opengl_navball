#ifndef TELEMACHUS
#define TELEMACHUS

#include <curl/curl.h>

struct Storage {
	char   *string;
	size_t length;
	unsigned int    alloced;
};

class Telemachus {
	public:
		Telemachus();
		~Telemachus();
		bool getPitchHeadingRoll(double *pitch, double *heading, double *roll);
	private:
		CURL *curl;
		struct Storage storage;
};

#endif
