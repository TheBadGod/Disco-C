#include <libwebsockets.h>

typedef struct client_websocket client_websocket_t;

typedef void (*callback_receive_fn)(struct client_websocket *, char *, size_t);

struct websocket_callbacks {
    callback_receive_fn on_receive;
};

typedef struct client_websocket {
    struct websocket_callbacks *callbacks;
    struct lws_context *context;
    struct lws *wsi;
} client_websocket_t;

int websocket_send(struct lws *wsi, char *data, size_t len);
int create_client_websocket(client_websocket_t *client, callback_receive_fn on_receive);
int websocket_test();