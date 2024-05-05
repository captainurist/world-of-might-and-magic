#pragma once

#include <Library/Fsm/FSMState.h>
#include <GUI/GUIEnums.h>

#include <string>
#include <memory>

class IMovie;

class VideoState : public FSMState {
 public:
    enum class Type {
        VIDEO_LOGO,
        VIDEO_INTRO
    };
    using enum Type;

    VideoState(Type type, std::string_view videoFileName);
    virtual FSMAction update() override;
    virtual FSMAction enter() override;
    virtual void exit() override;

 private:
    virtual bool mousePressEvent(const PlatformMouseEvent *event) override;
    virtual bool keyPressEvent(const PlatformKeyEvent *event) override;

    bool shouldSkip() const;

    Type _type = VIDEO_LOGO;
    std::string _videoFileName;
    std::shared_ptr<IMovie> _movie;
    ScreenType _previousScreenType{};
    bool _isPaused{};
    bool _skipVideo{};
};
