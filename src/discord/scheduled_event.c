#include "structures/scheduled_event.h"
#include "structures/structure.h"
#include "../web/request.h"

void *disco_create_scheduled_event_struct_from_json(cJSON *data) {
    ALLOC_STRUCT(discord_scheduled_event, event);
    cJSON *tmp = NULL;

    event->id = get_string_from_json(data, "id");
    event->guild_id = get_string_from_json(data, "guild_id");
    event->channel_id = get_string_from_json(data, "channel_id");
    event->creator_id = get_string_from_json(data, "creator_id");
    event->name = get_string_from_json(data, "name");
    event->description = get_string_from_json(data, "description");
    event->scheduled_start_time = get_string_from_json(data, "scheduled_start_time");
    event->scheduled_end_time = get_string_from_json(data, "scheduled_end_time");
    event->privacy_level = get_int_from_json(data, "privacy_level", GUILD_ONLY);
    event->status = get_int_from_json(data, "status", SCHEDULED);
    event->entity_type = get_int_from_json(data, "entity_type", STATUS_UNKNOWN);
    event->entity_id = get_string_from_json(data, "entity_id");

    tmp = cJSON_GetObjectItem(data, "entity_metadata");
    if(tmp)
        event->entity_metadata = disco_create_entity_metdata_from_json(tmp);

    tmp = cJSON_GetObjectItem(data, "creator");
    if(tmp)
        event->creator = disco_create_user_struct_json(tmp);

    event->user_count = get_int_from_json(data, "user_count", 0);
    event->image = get_string_from_json(data, "image");

    return event;
}

void disco_destroy_scheduled_event(struct discord_scheduled_event *event) {
    if(!event)
        return;
    if(event->id)
        free(event->id);
    if(event->guild_id)
        free(event->guild_id);
    if(event->channel_id)
        free(event->channel_id);
    if(event->creator_id)
        free(event->creator_id);
    if(event->name)
        free(event->name);
    if(event->description)
        free(event->description);
    if(event->scheduled_start_time)
        free(event->scheduled_start_time);
    if(event->scheduled_end_time)
        free(event->scheduled_end_time);
    if(event->entity_id)
        free(event->entity_id);
    if(event->entity_metadata)
        disco_destroy_entity_metadata(event->entity_metadata);
    if(event->creator)
        disco_destroy_user(event->creator);
    if(event->image)
        free(event->image);
    
    free(event);
}

struct discord_scheduled_event **disco_get_scheduled_events_for_guild(char *guild_id, int *size) {
    char endpoint[256];
    snprintf(endpoint, 256, "/guilds/%s/scheduled-events", guild_id);
    *size = 0;

    char *response;
    CURLcode res = request(endpoint, &response, NULL, REQUEST_GET);
    if(res == CURLE_OK) {
        cJSON *res_json = cJSON_Parse(response);

        if(res_json) {
            *size = cJSON_GetArraySize(res_json);

            struct discord_scheduled_event **array = (struct discord_scheduled_event**) 
                malloc(sizeof(void*) * (long unsigned int)(*size));
            
            int i = 0;
            cJSON *cur = NULL;
            cJSON_ArrayForEach(cur, res_json) {
                array[i++] = disco_create_scheduled_event_struct_from_json(cur);
            }
            cJSON_Delete(res_json);

            return array;
        }
    } else {
        d_log_err("Unable to fetch the scheduled events, Error code: %d\n", res);

        return NULL;
    }

    return NULL;
}

void disco_free_scheduled_event_array(struct discord_scheduled_event **array, int size) {
    if(!array || size <= 0)
        return;
    for(int i = 0; i < size; i++) {
        disco_destroy_scheduled_event(array[i]);
    }
    free(array);
}

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
        char *image,
        struct discord_scheduled_event **out_struct) {
    char endpoint[256];
    snprintf(endpoint, 256, "/guilds/%s/scheduled-events", guild_id);

    char *response;
    cJSON *json = cJSON_CreateObject();
    if(channel_id)
        cJSON_AddItemToObject(json, "channel_id", cJSON_CreateString(channel_id));
    else if(entity_type != EXTERNAL) {
        d_log_err("Channel id is only optional if external is true\n");
        goto err;
    }

    if(metadata)
        cJSON_AddItemToObject(json, "entity_metadata", disco_serialize_entity_metadata(metadata));
    else if(entity_type == EXTERNAL) {// only optional if external is false
        d_log_err("Metadata is always required if external is true\n");
        goto err;
    }

    if(name)
        cJSON_AddItemToObject(json, "name", cJSON_CreateString(name));
    else {
        d_log_err("Event name is always required\n");
        goto err;
    }

    cJSON_AddItemToObject(json, "privacy_level", cJSON_CreateNumber(privacy));

    if(scheduled_start_time)
        cJSON_AddItemToObject(json, "scheduled_start_time", cJSON_CreateString(scheduled_start_time));
    else {
        d_log_err("Start time is always required\n");
        goto err;
    }

    if(scheduled_end_time)
        cJSON_AddItemToObject(json, "scheduled_end_time", cJSON_CreateString(scheduled_end_time));
    else if(entity_type == EXTERNAL) {
        d_log_err("End time is always required if external is true\n");
        goto err;
    }

    if(description)
        cJSON_AddItemToObject(json, "description", cJSON_CreateString(description));

    cJSON_AddItemToObject(json, "entity_type", cJSON_CreateNumber(entity_type));

    if(image)
        cJSON_AddItemToObject(json, "image", cJSON_CreateString(image));

    CURLcode res = request(endpoint, &response, json, REQUEST_POST);
    if(res == CURLE_OK) {
        if(out_struct) {
            cJSON *res_json = cJSON_Parse(response);
            *out_struct = (struct discord_scheduled_event*) 
                disco_create_scheduled_event_struct_from_json(res_json);
            cJSON_Delete(res_json);
        }
    } else {
        d_log_err("Unable to post a scheduled event, Error code: %d\n", res);
    }

err:
    cJSON_Delete(json);
}
