#pragma once

#include <AL/al.h>

#include "Library/Pcm/PcmEnums.h"
#include "Library/Audio/Interface/AudioEnums.h"

ALenum translatePcmFormat(PcmBitness bitness, PcmChannels channels);
AudioState translateOpenALSourceState(ALenum state);
