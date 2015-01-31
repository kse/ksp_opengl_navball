#ifndef TELEMACHUS
#define TELEMACHUS

#include <libwebsockets.h>

class Telemachus {
	private:
		struct libwebsocket_context *wsContext;
		struct libwebsocket_protocols wsProtocols[2];

	public:
		Telemachus();
		~Telemachus();
		bool getPitchHeadingRoll(double *pitch, double *heading, double *roll);
};

#endif
