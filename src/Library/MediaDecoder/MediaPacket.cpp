#include "MediaPacket.h"

MediaType MediaPacket::type() const {
    switch (_data.index()) {
    default: assert(false); [[fallthrough]];
    case 0: return MEDIA_NONE;
    case 1: return MEDIA_AUDIO;
    case 2: return MEDIA_VIDEO;
    }
}
