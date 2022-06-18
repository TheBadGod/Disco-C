#ifndef DISCORD_ENTITY
#define DISCORD_ENTITY

#include <cJSON/cJSON.h>

// Guild Scheduled Event Entity Types
enum Discord_Scheduled_Event_Entity_Type {
    STATUS_UNKNOWN = 0,
    STAGE_INSTANCE = 1,
    VOICE = 2,
    EXTERNAL = 3,
};

struct discord_entity_metadata {
    char *location;
};

void *disco_create_entity_metdata_from_json(cJSON *data);
void disco_destroy_entity_metadata(struct discord_entity_metadata *entity);

#endif
