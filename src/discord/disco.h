#ifndef DISCO
#define DISCO

#include "../utils/t_pool.h"
#include "../web/websocket.h"
#include "structures/structure.h"
#include "intents.h"

typedef struct websocket_client websocket_client_t;
typedef struct bot_client bot_client_t;
struct discord_message;

typedef struct disco_event_callbacks {
    void (*on_ready)(struct bot_client *);
    void (*on_resumed)(struct bot_client *);
    void (*on_message)(struct bot_client *, struct discord_message *message);
    void (*on_message_edit)(struct bot_client *, struct discord_message *old, struct discord_message *new);
    /**
     * @brief Function called if a message event is called. The message ID and channel ID are always given.
     * If the message was sent in a guild, the guild ID is also included. If the message was in cache before being
     * deleted, the message struct is also included.
     *
     */
    void (*on_message_delete)(struct bot_client *, char *message_id, char *channel_id, char *guild_id, struct discord_message *);
    void (*on_channel_create)(struct bot_client *, struct discord_channel *channel);
    void (*on_channel_update)(struct bot_client *, struct discord_channel *old, struct discord_channel *new);
    void (*on_channel_delete)(struct bot_client *, char *channel_id, char *guild_id, struct discord_channel *);
} disco_event_callbacks_t;

typedef struct bot_client {
    websocket_client_t *websocket_client;
    disco_event_callbacks_t *callbacks;
    t_pool_t *thread_pool;
    long heartbeat_latency;
    
    char *token;
    int intents;
    struct discord_user *user;
} bot_client_t;

/**
 * @brief High level abstraction which starts the bot.
 *
 * @param callbacks A callbacks object containing the callbacks to
 * the event functions.
 * @param token The Discord bot token which is only visible once you
 * create the bot account (Or when you reset the token)
 * @param intents The intents for the bot (As in which capabilities
 * your bot should have)
 */
void disco_start_bot(disco_event_callbacks_t *callbacks, char *token, int intents);

/**
 * @brief Frees up memory for a bot instance.
 *
 * @param bot Bot instance
 */
void disco_free_bot(bot_client_t *bot);

char *get_string_from_json(cJSON *data, const char *name);
/**
 * @brief Get a bool from json object
 *
 * @param data
 * @param name
 * @param default_ The default value incase the field doesn't exist
 * @return int
 */
bool get_bool_from_json(cJSON *data, const char *name, int default_);
/**
 * @brief Get an int from json object
 *
 * @param data
 * @param name
 * @param default_ The default value incase the field doesn't exist
 * @return bool
 */
int get_int_from_json(cJSON *data, const char *name, int default_);

typedef void *(*disco_struct_fn)(cJSON *);
int get_array_from_json(cJSON *data, const char *name, void ***array, disco_struct_fn func);

#endif
