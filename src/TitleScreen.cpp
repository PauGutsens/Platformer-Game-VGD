#include "TitleScreen.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "GuiManager.h"
#include "SDL2/SDL.h"

TitleScreen::TitleScreen() : Module(), backgroundTexture(nullptr), playButton(nullptr), shouldStartGame(false) {
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

        // Crear el botón con dimensiones seguras
        SDL_Rect buttonRect = {
            w / 2 - 100,  // x
            h / 2 - 25,   // y
            200,        // width
            50          // height
        };

        playButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
            GuiControlType::BUTTON,
            1,
            "PLAY",
            buttonRect,
            this
        );

        if (!playButton) {
            LOG("Failed to create play button!");
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        LOG("Exception in TitleScreen::Start(): %s", e.what());
        return false;
    }
}
bool TitleScreen::Update(float dt) {
    if (!backgroundTexture) {
        return false;
    }

    // Obtener dimensiones de la ventana
    int w, h;
    Engine::GetInstance().window.get()->GetWindowSize(w, h);

    // Dibuja el fondo
    SDL_Rect fullscreen = { 0, 0, w, h };
    Engine::GetInstance().render.get()->DrawTexture(backgroundTexture, 0, 0, &fullscreen);

    // Si el botón fue presionado, deshabilitarlo
    if (playButton && shouldStartGame) {
        playButton->state = GuiControlState::DISABLED;
        playButton = nullptr;  // Aseguramos que no se vuelva a usar
    }

    return true;
}

bool TitleScreen::CleanUp() {
    if (backgroundTexture != nullptr) {
        Engine::GetInstance().textures.get()->UnLoad(backgroundTexture);
        backgroundTexture = nullptr;
    }
    return true;
}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control) {
    if (control->id == 1) { // PLAY button
        shouldStartGame = true;
        playButton->state = GuiControlState::DISABLED;  // Deshabilitar inmediatamente
    }
    return true;
}
