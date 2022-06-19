#include "example/example.h"
#include "src/discord/disco.h"

#include "config.h"

int main() {
    // Enable logging
    d_set_log_level(D_LOG_ERR | D_LOG_NORMAL | D_LOG_NOTICE | D_LOG_DEBUG);

    // init to 0. Without this some errors could show up
    disco_event_callbacks_t callbacks = {0};
    callbacks.on_ready = &example_on_ready;
    callbacks.on_message = &example_on_message;
    callbacks.on_message_edit = &example_on_edit;
    callbacks.on_message_delete = &example_on_delete;
    callbacks.on_channel_create = &example_channel_create;
    callbacks.on_channel_update = &example_channel_update;
    callbacks.on_channel_delete = &example_channel_delete;

    // starts the bot. This function blocks
    disco_start_bot(&callbacks, DISCORD_TOKEN, INTENT_GUILDS | INTENT_GUILD_MESSAGES | INTENT_GUILD_SCHEDULED_EVENTS);

    return 0;
}
