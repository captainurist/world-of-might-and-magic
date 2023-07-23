#pragma once

#include "Engine/Objects/ItemEnums.h"
#include "Engine/Pid.h"

#include "Utility/Geometry/Vec.h"

struct SpawnPoint {
    Vec3i vPosition;
    uint16_t uRadius = 32;
    ObjectType uKind = OBJECT_None;
    ITEM_TREASURE_LEVEL uItemIndex = ITEM_TREASURE_LEVEL_INVALID;
    uint16_t uMonsterIndex = 0;
    uint16_t uAttributes = 0;
    unsigned int uGroup = 0;
};
