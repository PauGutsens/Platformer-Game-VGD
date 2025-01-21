#pragma once
#include "Module.h"
#include "GuiControlButton.h"
#include "LOG.h"
struct SDL_Texture;

class TitleScreen : public Module {
public:
    TitleScreen();
    virtual ~TitleScreen();

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;
    bool OnGuiMouseClickEvent(GuiControl* control);

    bool ShouldTransitionToGame() const { return shouldStartGame; }

private:
    SDL_Texture* backgroundTexture;
    GuiControlButton* playButton;
    bool shouldStartGame;
};