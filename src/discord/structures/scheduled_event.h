#ifndef DISCO_SCHEDULED_EVENT
#define DISCO_SCHEDULED_EVENT

#include <cJSON/cJSON.h>
#include "user.h"
#include "entity.h"

enum Discord_Scheduled_Event_Status {
    SCHEDULED = 0,
    ACTIVE = 1,
    COMPLETED = 2,
    CANCELED = 3,
};

enum Discord_Scheduled_Event_Privacy_Level {
    GUILD_ONLY = 2,
};

struct discord_scheduled_event {
    char *id;
    char *guild_id;
    char *channel_id;
    char *creator_id;
    char *name;
    char *description;
    char *scheduled_start_time;
    char *scheduled_end_time;
    enum Discord_Scheduled_Event_Privacy_Level privacy_level;
    enum Discord_Scheduled_Event_Status status;
    enum Discord_Scheduled_Event_Entity_Type entity_type;
    char *entity_id;
    void *entity_metadata;
    struct discord_user *creator;
    int user_count;
    char *image;
};

void *disco_create_scheduled_event_struct_from_json(cJSON *data);
void disco_destroy_scheduled_event(struct discord_scheduled_event *event);

struct discord_scheduled_event **disco_get_scheduled_events_for_guild(char *guild_id, int *size);
void disco_free_scheduled_event_array(struct discord_scheduled_event **array, int size);

void disco_guild_create_scheduled_event(
        char *guild_id,
        char *channel_id , // optional for EXTERNAL
        struct discord_entity_metadata *metadata,
        char *name,
        enum Discord_Scheduled_Event_Privacy_Level privacy,
        char *scheduled_start_time,
        char *scheduled_end_time,
        char *description,
        enum Discord_Scheduled_Event_Entity_Type entity_type,
        char *image);

#endif
