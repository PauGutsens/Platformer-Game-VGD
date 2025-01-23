#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include "Item.h"
#include "Physics.h"
#include "GuiControlButton.h"
#include "TitleScreen.h"



struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	Vector2D GetPlayerPosition();

	//L15 TODO 1: Implement the Load function
	void LoadState();
	void SaveState();
	//L15 TODO 2: Implement the Save function
	void CoinNum();
	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);
	bool pendingToLoad;
	bool InitGame();  // Nueva función para inicializar el juego



public:
	// Get tilePosDebug value
	std::string GetTilePosDebug() {
		return tilePosDebug;

	}
	int coinFxId;


private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;
	std::vector<Item*> itemList;  // Lista para mantener track de los items
	SDL_Texture* coinTexture; // Textura para la moneda
	TTF_Font* font; // Atributo para la fuente
	SDL_Texture* coinIconTexture;

	//L03: TODO 3b: Declare a Player attribute
	Player *player;
	std::vector<Enemy*> enemyList;
	// L16: TODO 2: Declare a GUI Control Button 
	GuiControlButton* guiBt;

	/*bool gameStarted;
	SDL_Texture* startScreenTexture;
	GuiControlButton* startButton;*/
	TitleScreen* titleScreen;
	bool gameStarted;
};
