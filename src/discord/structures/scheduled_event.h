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

#endif
