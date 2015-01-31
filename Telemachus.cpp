#include <jsoncpp/json/json.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "Telemachus.hpp"

typedef struct {
	float pitch;
	float heading;
	float roll;
} measurement;

int
wsSendMessage(struct libwebsocket *wsi, std::string msg) {
	unsigned char *buf;
	buf = (unsigned char*)malloc(msg.length() +
			LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING);

	memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], msg.c_str(), msg.length());
	int res = libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], msg.length(), LWS_WRITE_TEXT);
	(void)res;
	free(buf);
	return res;
}

bool Telemachus::getPitchHeadingRoll(double *pitch, double *heading, double *roll) {
	
	libwebsocket_service(wsContext, 0);
	measurement *m;
	m = (measurement*)libwebsocket_context_user(wsContext);

	*pitch = m->pitch;
	*heading = m->heading;
	*roll = m->roll;

	return true;
}

int wsCallback(struct libwebsocket_context * context,
		struct libwebsocket *wsi,
		enum libwebsocket_callback_reasons reason,
		void *user, void *in, size_t len) {
	int res = 0;

	Json::Value root;
	Json::Reader reader;
	bool success;
	measurement *m;

	switch(reason) {
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			// TODO: Check results from call more reliably.
			res = wsSendMessage(wsi, "{\"+\":[\"n.rawheading\",\"n.rawpitch\",\"n.rawroll\"]}");
			assert(res != -1);
			res = wsSendMessage(wsi, "{\"rate\":30}");
			assert(res != -1);

			m = (measurement*)libwebsocket_context_user(context);
			m->pitch = 0.0f;
			m->heading = 0.0f;
			m->roll = 0.0f;

			break;
		case LWS_CALLBACK_CLIENT_RECEIVE:
		case LWS_CALLBACK_RECEIVE:
			//unsigned i;
			//for (i = 0; i < len; i++) {
			//	putchar(((char*)in)[i]);
			//}
			//putchar('\n');

			m = (measurement*)libwebsocket_context_user(context);
			success = reader.parse((const char*)in, root);

			if (!success) {
				std::cout << reader.getFormattedErrorMessages();
			} else {
				m->pitch = root.get("n.rawpitch", Json::Value(0.0)).asDouble();
				m->heading = root.get("n.rawheading", Json::Value(0.0)).asDouble();
				m->roll = root.get("n.rawroll", Json::Value(0.0)).asDouble();
			}

			break;

		case LWS_CALLBACK_PROTOCOL_DESTROY:
			printf("WS Protocol Destroyed\n");
			break;

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			printf("Unable to connect to websocketserver\n");
			break;

		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
			printf("WS filter protocol\n");
			return 0;
		case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
			printf("WS filter network\n");
			return 0;

		case LWS_CALLBACK_CLOSED:
			printf("Callback closed\n");
			break;

		case LWS_CALLBACK_WSI_CREATE:
			printf("WS create notification\n");
			break;
		case LWS_CALLBACK_WSI_DESTROY:
			printf("WS destroy notification\n");
			break;
		default:
			//printf("WS Default %d\n", reason)
			break;
	}

	return 0;
}

Telemachus::Telemachus() {
	//memset(wsProtocols, 0, 2*sizeof(struct libwebsocket_protocols));
	wsProtocols[0].name     = "Telemachus";
	wsProtocols[0].callback = wsCallback;
	wsProtocols[0].per_session_data_size = 0;
	wsProtocols[0].rx_buffer_size = 4096;
	wsProtocols[0].protocol_index = 0;
	wsProtocols[0].owning_server = 0;

	wsProtocols[1].name     = "Dummy";
	wsProtocols[1].callback = NULL;

	//lws_set_log_level(LLL_INFO | LLL_WARN | LLL_HEADER | LLL_NOTICE, NULL);
	lws_set_log_level(0, NULL);

	struct lws_context_creation_info wsCri;
	memset(&wsCri, 0, sizeof(struct lws_context_creation_info));

	wsCri.port = CONTEXT_PORT_NO_LISTEN;
	wsCri.protocols = wsProtocols;
	wsCri.uid = -1;
	wsCri.gid = -1;
	wsCri.options = 0;
	wsCri.ka_time = 30;
	wsCri.iface = NULL;
	wsCri.extensions = NULL;
	wsCri.token_limits = NULL;
	wsCri.ssl_cert_filepath = NULL;
	wsCri.ssl_private_key_filepath = NULL;
	wsCri.ssl_ca_filepath = NULL;
	wsCri.ssl_cipher_list = NULL;
	wsCri.ka_probes = 2;
	wsCri.ka_probes = 2;
	wsCri.user = malloc(sizeof(measurement));

	wsContext = libwebsocket_create_context(&wsCri);
	struct libwebsocket *ws;
	ws = libwebsocket_client_connect(wsContext, "127.0.0.1",
			8085,
			0,
			"/datalink",
			"Telemachus",
			"Navball",
			NULL,
			-1);

	if (ws == NULL) {
		fprintf(stderr, "WS Creation failed\n");
		exit(EXIT_FAILURE);
	}
}

Telemachus::~Telemachus() {
	free(libwebsocket_context_user(wsContext));
	libwebsocket_context_destroy(wsContext);
}
