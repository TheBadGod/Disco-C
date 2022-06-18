#ifndef DISCO_STRUCTURE
#define DISCO_STRUCTURE

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include "../../utils/disco_logging.h"

// Technically not all needed as they include each other, but oh well
#include "application.h"
#include "attachment.h"
#include "channel.h"
#include "component.h"
#include "embed.h"
#include "emoji.h"
#include "entity.h"
#include "guild.h"
#include "message.h"
#include "message_activity.h"
#include "permission.h"
#include "reaction.h"
#include "role.h"
#include "scheduled_event.h"
#include "sticker.h"
#include "thread.h"
#include "user.h"

#include "../disco.h"

#define ALLOC_STRUCT(struc, name) struct struc *name = (struct struc*) calloc(1, sizeof(struct struc))


#endif
