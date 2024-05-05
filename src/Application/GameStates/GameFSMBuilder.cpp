#include "GameFSMBuilder.h"

#include <Engine/Engine.h>
#include <Library/Fsm/FSMBuilder.h>

#include <utility>
#include <memory>

#include "VideoState.h"

std::unique_ptr<FSM> GameFSMBuilder::buildFSM() {
    FSMBuilder fsmBuilder;
    _buildIntroVideoSequence(fsmBuilder);

    auto fsm = fsmBuilder.build();
    fsm->jumpToState("3DOVideo");
    return fsm;
}

void GameFSMBuilder::_buildIntroVideoSequence(FSMBuilder &builder) {
    builder
    .state<VideoState>("3DOVideo", VideoState::VIDEO_LOGO, "3dologo")
        .on("videoEnd").jumpTo("NWCVideo")

    .state<VideoState>("NWCVideo", VideoState::VIDEO_LOGO,"new world logo")
        .on("videoEnd").jumpTo("JVCVideo")

    .state<VideoState>("JVCVideo", VideoState::VIDEO_LOGO,"jvc")
        .on("videoEnd").jumpTo("IntroVideo")

    .state<VideoState>("IntroVideo", VideoState::VIDEO_INTRO,"Intro")
        .on("videoEnd").jumpTo(FSM::exitState);
}
