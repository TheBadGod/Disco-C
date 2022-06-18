#include "structures/scheduled_event.h"
#include "structures/structure.h"


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
    event->privacy_level = (enum Discord_Scheduled_Event_Privacy_Level) get_int_from_json(data, "privacy_level", GUILD_ONLY);
    event->status = (enum Discord_Scheduled_Event_Status) get_int_from_json(data, "status", SCHEDULED);
    event->entity_type = (enum Discord_Scheduled_Event_Entity_Type) get_int_from_json(data, "entity_type", STATUS_UNKNOWN);
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
