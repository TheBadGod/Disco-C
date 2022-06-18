#include "example.h"
#include "sys/time.h"

void example_on_ready(bot_client_t *bot) {
    printf("====================================\n");
    printf("Successfully logged in\n");
    printf("Username:\t%s\n", bot->user->username);
    printf("User ID:\t%s\n", bot->user->id);
    printf("====================================\n\n");

    char *guild_id = "813371933667885076";

    int size;
    struct discord_scheduled_event **events = disco_get_scheduled_events_for_guild(guild_id, &size);

    printf("There exist %d events for guild %s:\n", size, guild_id);
    for(int i = 0; i < size; i++) {
        if(events[i])
            printf("  %s\n", events[i]->name);
    }
    disco_free_scheduled_event_array(events, size);

    char start[256];
    char end[256];
    time_t now;
    time(&now);

    now += 10;
    strftime(start, 256, "%FT%TZ", gmtime(&now));
    now += 1000;
    strftime(end, 256, "%FT%TZ", gmtime(&now));

    struct discord_scheduled_event *event;

    struct discord_entity_metadata metadata = {
        "This is the location"
    };
    disco_guild_create_scheduled_event(
            guild_id,
            NULL, // channel id, can only be null if EXTERNAL
            &metadata,
            "Event from C",
            GUILD_ONLY,
            start,
            end,
            "Test event, created using Disco-C", // Description, optional
            EXTERNAL, // Since we don't have a channel id we need it to be external
            NULL, // No image
            &event // Don't put the output into a struct
    );
    
    printf("Created event  : %s\n", event->name);

    disco_destroy_scheduled_event(event);
}

void example_on_message(bot_client_t *bot, struct discord_message *message) {
    (void)bot;
    fprintf(stderr, "Received a message: %s\n", message ? message->content : "(null)");
    if (message->content) { // message content is NULL if there's none
        if (message->author && message->author->bot) {
            fprintf(stderr, "User is a bot. Ignoring\n");
            return;
        }
        // example of a ping command
        if (strncmp(message->content, "!ping", 6) == 0) {
            struct timeval stop, start;
            gettimeofday(&start, NULL);
            // sends the initial message to a channel
            struct discord_message *msg = disco_channel_send_message(bot, "Pinging...", message->channel_id, NULL, 1);
            gettimeofday(&stop, NULL);
            char time_passed[32];
            long delta = (stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_usec - start.tv_usec) / 1000;
            sprintf(time_passed, "Ping: %lu ms\nHeartbeat: %lu ms", delta, bot->heartbeat_latency);
            // edits the message right after the message has been sent to check the latency
            disco_channel_edit_message(bot, time_passed, msg->channel_id, msg->id, NULL);
            // don't forget to destroy the message in the end to avoid memory leaks
            disco_destroy_message(msg);

        } else if (strncmp(message->content, "!exit", 6) == 0) {
            // softly ends the bot
            websocket_close(bot);

        } else if (strncmp(message->content, "!r", 3) == 0) {
            // reconnects the websocket
            websocket_reconnect(bot);
        }
    }
}

void example_on_edit(bot_client_t *bot, struct discord_message *old, struct discord_message *new) {
    char content[80];
    sprintf(content, "Message %s was edited. Message in cache: %s", new->id, old ? "Yes" : "No");
    disco_channel_send_message(bot, content, new->channel_id, NULL, 0);
}

void example_on_delete(struct bot_client *bot, char *message_id, char *channel_id, char *guild_id, struct discord_message *message) {
    if (message) {
        char content[50];
        sprintf(content, "Cache: Yes\nID: `%s`", message_id);
        disco_channel_send_message(bot, content, channel_id, NULL, 0);
    } else {
        char content[100];
        sprintf(content, "Cache: No\nID: %s, channel ID: %s, guild ID: %s", message_id, channel_id, guild_id);
        disco_channel_send_message(bot, content, channel_id, NULL, 0);
    }
}
