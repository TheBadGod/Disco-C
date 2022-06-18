#include "structures/entity.h"
#include "structures/structure.h"

void *disco_create_entity_metdata_from_json(cJSON *data) {
    ALLOC_STRUCT(discord_entity_metadata, entity);

    entity->location = get_string_from_json(data, "location");

    return entity;
}

void disco_destroy_entity_metadata(struct discord_entity_metadata *entity) {
    if(!entity)
        return;
    free(entity);
}

cJSON *disco_serialize_entity_metadata(struct discord_entity_metadata *entity) {
    cJSON *object = cJSON_CreateObject();
    if(entity && entity->location)
        cJSON_AddItemToObject(object, "location", cJSON_CreateString(entity->location));
    return object;
}
