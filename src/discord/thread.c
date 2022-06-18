#include "structures/thread.h"
#include "structures/structure.h"

void *disco_create_thread_metadata_struct_json(cJSON *data) {
    ALLOC_STRUCT(discord_thread_metadata, metadata);

    metadata->archived = get_bool_from_json(data, "archived", 0);
    metadata->auto_archive_duration = get_int_from_json(data, "auto_archive_duration", 0);
    metadata->archive_timestamp = get_string_from_json(data, "archive_timestamp");
    metadata->locked = get_bool_from_json(data, "locked", 0);

    // Only for private threads => for public ones "invitable" should be true
    metadata->invitable = get_bool_from_json(data, "invitable", 1);

    // Only available for "new" threads (Created after 2022-01-09)
    metadata->create_timestamp = get_string_from_json(data, "create_timestamp");

    return metadata;
}
void disco_destroy_thread_metadata(struct discord_thread_metadata *metadata) {
    if(!metadata)
        return;
    if(metadata->archive_timestamp) 
        free(metadata->archive_timestamp);
    if(metadata->create_timestamp)
        free(metadata->create_timestamp);
    free(metadata);
}

void *disco_create_thread_member_struct_json(cJSON *data) {
    struct discord_thread_member *member = (struct discord_thread_member*)calloc(1, sizeof(struct discord_thread_member));

    // Thread id, omitted on the member sent within each thread in the GUILD_CREATE event
    member->id = get_string_from_json(data, "id");
    // User id, omitted on the member sent within each thread in the GUILD_CREATE event
    member->user_id = get_string_from_json(data, "user_id");

    // When the user joined, always present
    member->join_timestamp = get_string_from_json(data, "join_timestamp");

    // flags
    member->flags = get_int_from_json(data, "flags", 0);

    return member;
}
void disco_destroy_thread_member(struct discord_thread_member *member) {
    if(!member)
        return;
    if(member->id)
        free(member->id);
    if(member->user_id)
        free(member->user_id);
    if(member->join_timestamp)
        free(member->join_timestamp);
    free(member);
}
