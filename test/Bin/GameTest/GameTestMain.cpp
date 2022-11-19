#include <gtest/gtest.h>

#include <thread>

#include "Application/Game.h"
#include "Application/GameFactory.h"

#include "Testing/Engine/TestState.h"
#include "Testing/Engine/TestStateHandle.h"
#include "Testing/Engine/TestPlatform.h"
#include "Testing/Game/GameWrapper.h"
#include "Testing/Game/GameTest.h"

void RunGameThread(TestState *state) {
    Log *log = EngineIoc::ResolveLogger();
    std::unique_ptr<Platform> platform = std::make_unique<TestPlatform>(Platform::CreateStandardPlatform(log), TestStateHandle(GameSide, state));

    std::shared_ptr<Application::GameConfig> config = std::make_shared<Application::GameConfig>("");
    config->Startup(); // TODO(captainurist): Reads from womm.ini, not good for tests
    config->debug.NoIntro.Set(true);
    config->debug.NoLogo.Set(true);
    config->window.MouseGrab.Set(false);

    std::shared_ptr<Application::Game> game = Application::GameFactory().CreateGame(platform.get(), config);

    game->Run();
}

void RunTestThread(TestState *state, int *exitCode) {
    GameWrapper gameWrapper(TestStateHandle(TestSide, state));
    GameTest::Init(&gameWrapper);

    gameWrapper.Tick(); // Let the game thread initialize everything.

    *exitCode = RUN_ALL_TESTS();
}

int PlatformMain(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    int exitCode = 0;
    TestState state;

    std::thread testThread(&RunTestThread, &state, &exitCode);
    RunGameThread(&state);

    // TODO: exit game.

    testThread.join();

    return exitCode;
}
