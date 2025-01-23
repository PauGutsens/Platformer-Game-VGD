#include "TitleScreen.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "GuiManager.h"
#include "SDL2/SDL.h"
#include "Scene.h"

TitleScreen::TitleScreen()
    : Module(), backgroundTexture(nullptr), playButton(nullptr), continueButton(nullptr),
    settingsButton(nullptr), creditsButton(nullptr), exitButton(nullptr), backButton(nullptr), shouldStartGame(false) {
    name = "titlescreen";
}

TitleScreen::~TitleScreen() {}

bool TitleScreen::Start()
{
    try {
        // Cargar la textura de fondo
        backgroundTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/inicial_screen.png");
        if (!backgroundTexture) {
            LOG("Failed to load background texture!");
            return false;
        }

        // Obtener dimensiones de la ventana
        int w = 800;  // valor por defecto
        int h = 600;  // valor por defecto
        Engine::GetInstance().window.get()->GetWindowSize(w, h);

        // Crear los botones con dimensiones seguras
        SDL_Rect playRect = { w / 2 - 90, h / 2 - 140, 125, 42 };
        SDL_Rect continueRect = { w / 2 - 90, h / 2 - 27, 125, 42 };
        SDL_Rect settingsRect = { w / 2 - 90, h / 2 + 86, 125, 42 };
        SDL_Rect creditsRect = { w / 2 - 55, h / 2 + 200, 55, 22 };
        SDL_Rect exitRect = { w / 2 - 55, h / 2 + 266, 55, 22 };

        SDL_Rect backRect = { w / 2 - 90, h / 2 + 160, 125, 42 }; 


        playButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 1, "PLAY", playRect, this);
        continueButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 2, "CONTINUE", continueRect, this);
        settingsButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 3, "SETTINGS", settingsRect, this);
        creditsButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 4, "CREDITS", creditsRect, this);
        exitButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 5, "EXIT", exitRect, this);
        backButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON, 6, "BACK", backRect, this);
        backButton->state = GuiControlState::DISABLED; 

        if (!playButton || !continueButton || !settingsButton || !creditsButton || !exitButton || !backButton) {
            LOG("Failed to create one or more buttons!");
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        LOG("Exception in TitleScreen::Start(): %s", e.what());
        return false;
    }
}

bool TitleScreen::Update(float dt)
{
    if (!backgroundTexture) {
        return false;
    }

    // Obtener dimensiones de la ventana
    int w, h;
    Engine::GetInstance().window.get()->GetWindowSize(w, h);

    // Dibuja el fondo
    SDL_Rect fullscreen = { 0, 0, w, h };
    Engine::GetInstance().render.get()->DrawTexture(backgroundTexture, 0, 0, &fullscreen);

    if (isInSettings && settingTexture) {
        int w, h;
        Engine::GetInstance().window.get()->GetWindowSize(w, h);

        SDL_Rect fullscreen = { 0, 0, w, h };
        Engine::GetInstance().render.get()->DrawTexture(settingTexture, 0, 0, &fullscreen);
    }
    if (isInCredits && creditsTexture) {
        int w, h;
        Engine::GetInstance().window.get()->GetWindowSize(w, h);

        SDL_Rect fullscreen = { 0, 0, w, h };
        Engine::GetInstance().render.get()->DrawTexture(creditsTexture, 0, 0, &fullscreen);

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RETURN]) {
            LOG("Returning to title screen from credits.\n");
            isInCredits = false;

            playButton->state = GuiControlState::NORMAL;
            continueButton->state = GuiControlState::NORMAL;
            settingsButton->state = GuiControlState::NORMAL;
            creditsButton->state = GuiControlState::NORMAL;
            exitButton->state = GuiControlState::NORMAL;
            backButton->state = GuiControlState::DISABLED;

            Engine::GetInstance().textures.get()->UnLoad(creditsTexture);
            creditsTexture = nullptr;
        }
    }
    return true;
}

bool TitleScreen::CleanUp()
{
    if (backgroundTexture != nullptr) {
        Engine::GetInstance().textures.get()->UnLoad(backgroundTexture);
        backgroundTexture = nullptr;
    }
    if (settingTexture != nullptr) {
        Engine::GetInstance().textures.get()->UnLoad(settingTexture);
        settingTexture = nullptr;
    }
    if (creditsTexture != nullptr) {
        Engine::GetInstance().textures.get()->UnLoad(creditsTexture);
        creditsTexture = nullptr;
    }
    return true;
}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->id) {
    case 1: // PLAY button
        shouldStartGame = true;
        playButton->state = GuiControlState::DISABLED;
        continueButton->state = GuiControlState::DISABLED;
        settingsButton->state = GuiControlState::DISABLED;
        exitButton->state = GuiControlState::DISABLED;
        creditsButton->state = GuiControlState::DISABLED;
        backButton->state = GuiControlState::DISABLED;

        break;
    case 2: // CONTINUE button
        LOG("Continue button clicked (implement logic to resume game).\n");
        shouldStartGame = true;
        playButton->state = GuiControlState::DISABLED;
        continueButton->state = GuiControlState::DISABLED;
        settingsButton->state = GuiControlState::DISABLED;
        exitButton->state = GuiControlState::DISABLED;
        creditsButton->state = GuiControlState::DISABLED;
        backButton->state = GuiControlState::DISABLED;

        break;
    case 3: // SETTINGS button
        LOG("Settings button clicked (switching to settings screen).\n");
        settingTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/menu_screen.png");
        if (!settingTexture) {
            LOG("Failed to load setting texture!");
            return false;
        }
        isInSettings = true;
        playButton->state = GuiControlState::DISABLED;
        continueButton->state = GuiControlState::DISABLED;
        settingsButton->state = GuiControlState::DISABLED;
        creditsButton->state = GuiControlState::DISABLED;
        exitButton->state = GuiControlState::DISABLED;
        backButton->state = GuiControlState::NORMAL;

        break;
    case 4: // CREDITS button
        LOG("Credits button clicked (switching to credits screen).\n");
        creditsTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/credits_screen.png");
        if (!creditsTexture) {
            LOG("Failed to load credits texture!");
            return false;
        }

        isInCredits = true;

        // Desactiva los botones si es necesario
        playButton->state = GuiControlState::DISABLED;
        continueButton->state = GuiControlState::DISABLED;
        settingsButton->state = GuiControlState::DISABLED;
        creditsButton->state = GuiControlState::DISABLED;
        backButton->state = GuiControlState::DISABLED;
        exitButton->state = GuiControlState::DISABLED;
        break;
    case 5: // EXIT button
        LOG("Exit button clicked (exit application).\n");
        SDL_Quit();
        exit(0); // Forzar salida del programa
        break;
    case 6: // BACK button
        LOG("Back button clicked (returning to title screen).\n");

        isInSettings = false;
        playButton->state = GuiControlState::NORMAL;
        continueButton->state = GuiControlState::NORMAL;
        settingsButton->state = GuiControlState::NORMAL;
        creditsButton->state = GuiControlState::NORMAL;
        exitButton->state = GuiControlState::NORMAL;
        backButton->state = GuiControlState::DISABLED;

        // Liberar textura de settings si existe
        if (settingTexture != nullptr) {
            Engine::GetInstance().textures.get()->UnLoad(settingTexture);
            settingTexture = nullptr;
        }

        break;
    default:
        LOG("Unknown button clicked!\n");
        break;
    }
    return true;
}

bool TitleScreen::ShouldTransitionToGame() const
{
    return shouldStartGame;
}
