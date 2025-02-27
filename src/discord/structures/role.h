#ifndef DISCO_ROLE
#define DISCO_ROLE

#include <stdbool.h>

// https://discord.com/developers/docs/topics/permissions#role-object-role-tags-structure
struct discord_role_tags {
    char *bot_id;
    char *integration_id;
    bool premium_subscriber;
};

// TODO implement
void *disco_create_role_tags_struct_json(cJSON *data);
void disco_destroy_role_tags(struct discord_role_tags *tags);

// https://discord.com/developers/docs/topics/permissions#role-object
struct discord_role {
    char *id;
    char *name;
    int color;
    bool hoist;
    char *icon;
    char *unicode_emoji;
    int position;
    char *permissions;
    bool managed;
    bool mentionable;
    struct discord_role_tags tags;
    int tags_count;
};

// TODO implement
void *disco_create_role_struct_json(cJSON *data);
void disco_destroy_role(struct discord_role *role);

#endif
