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
    bool OnGuiMouseClickEvent(GuiControl* control) override;

    bool ShouldTransitionToGame() const;

private:
    SDL_Texture* backgroundTexture;
    SDL_Texture* settingTexture;
    SDL_Texture* creditsTexture;
    GuiControlButton* playButton;
    GuiControlButton* continueButton;
    GuiControlButton* settingsButton;
    GuiControlButton* creditsButton;
    GuiControlButton* exitButton;
    GuiControlButton* backButton;

    bool shouldStartGame;
    bool isInSettings = false;
    bool isInCredits = false;

};
