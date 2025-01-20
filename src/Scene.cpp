#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene::Scene() : Module()
{
	name = "scene";
}



// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	

	// Initialize the game state
	//gameStarted = false;
	//if (!gameStarted)
	//{
	//	// Load start screen texture
	//	startScreenTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/inicial_screen.png");  // Replace with your actual texture path
	//	if (!startScreenTexture) {
	//		SDL_Log("Failed to load start screen texture!");
	//		return false;
	//	}
	//	// Create start button
	//	SDL_Rect startBtnPos = { 400, 300, 200, 50 };  // Adjust position and size as needed
	//	startButton = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(
	//		GuiControlType::BUTTON,
	//		2,  // Unique ID for the start button
	//		"PLAY",
	//		startBtnPos,
	//		this
	//	);
	//}
	//else if(gameStarted)
	//{

		//gameStarted = true;
		//Player initialization
		player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
		player->SetParameters(configParameters.child("entities").child("player"));

		// Crear items
		pugi::xml_node items = configParameters.child("entities").child("Items");
		for (pugi::xml_node itemNode = items.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
		{
			Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
			item->SetParameters(itemNode);
			itemList.push_back(item);  // Añadir a nuestra lista
		}

		// Create enemies
		for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy");
			enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
			enemy->SetParameters(enemyNode);
			enemyList.push_back(enemy);
		}
		// L16: TODO 2: Instantiate a new GuiControlButton in the Scene
		SDL_Rect btPos = { 520, 350, 120,20 };
		guiBt = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Menu", btPos, this);

		return ret;
	/*}*/
}

// Called before the first frame
bool Scene::Start()
{
	//if (!gameStarted)
	//{
	//	return true;  // Only load game elements when gameStarted is true
	//}
	//else if (gameStarted)
	//{


		//L06 TODO 3: Call the function to load the map. 
		Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

		// Texture to highligh mouse position 
		mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");

		// Initalize the camera position
		int w, h;
		Engine::GetInstance().window.get()->GetWindowSize(w, h);
		Engine::GetInstance().render.get()->camera.x = 0;
		Engine::GetInstance().render.get()->camera.y = 0;
		Engine::GetInstance().audio.get()->PlayMusic("Assets/Textures/Audio/Music/game-music-loop-3-144252.ogg", 0.1f);




		coinIconTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/goldCoin.png");  // Ajusta la ruta según donde tengas la textura

		SDL_Rect coinSection = { 30, 110, 32, 32 };  // Ajusta estos valores según el tamaño de tu textura
		Engine::GetInstance().render.get()->DrawTexture(coinIconTexture, 20, 20, &coinSection);




		return true;
	/*}*/
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//if (!gameStarted) {
	//	if (!startScreenTexture) {
	//		SDL_Log("Start screen texture is null!");
	//		return false;
	//	}

	//	i/*nt width, height;
	//	Engine::GetInstance().window.get()->GetWindowSize(width, height);
	//	SDL_Rect fullscreen = { 0, 0, width, height };

	//	Engine::GetInstance().render.get()->DrawTexture(startScreenTexture, 0, 0, &fullscreen);*/
	//	return true;
	//}

	//L03 TODO 3: Make the camera movement independent of framerate
	Engine::GetInstance().render.get()->camera.x = -player->position.getX() + 40;

	

	// L10 TODO 6: Implement a method that repositions the player in the map with a mouse click

	//Get mouse position and obtain the map coordinate
	//Get mouse position and obtain the map coordinate
	int scale = Engine::GetInstance().window.get()->GetScale();
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(mousePos.getX() - Engine::GetInstance().render.get()->camera.x / scale,
                                                                 mousePos.getY() - Engine::GetInstance().render.get()->camera.y / scale);

		//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(),mouseTile.getY());
	SDL_Rect rect = { 0,0,32,32 };

	// Solo dibujamos el highlight y procesamos el click si hay enemigos
	if (!enemyList.empty()) {
		Engine::GetInstance().render.get()->DrawTexture(mouseTileTex,
			highlightTile.getX(),
			highlightTile.getY(),
			&rect);

		// saves the tile pos for debugging purposes
		if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once) {
			tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
			once = true;
		}

		//If mouse button is pressed modify enemy position
		if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN) {
			enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
			enemyList[0]->ResetPath();
		}
	
	}
	
	

	if (player->isDead) {
		player->isDead = false; // Reset la bandera
		LoadState();  // Recargar el nivel
	}

	//font = TTF_OpenFont("Assets/Fonts/arial/arial.ttf", 24); // Carga la fuente con tamaño 24

	//if (font == nullptr) {
	//	SDL_Log("Error loading font: %s", TTF_GetError());
	//	return false; // Maneja el error si no se puede cargar la fuente
	//}
	//SDL_Color color = { 255, 255, 255, 255 }; // Color blanco para el texto
	//
	//// Posición en pantalla para el contador y la moneda
	//int coinX = 10; // Posición X para el contador
	//int coinY = 10; // Posición Y para el contador
	//int coinTextOffset = 40; // Espaciado entre la textura y el texto

	// Dibujar la textura de la moneda
	//Engine::GetInstance().render.get()->DrawTexture(coinTexture, coinX, coinY, nullptr);
	//Engine::GetInstance().render.get()->DrawText("X\t ")
	// Dibujar el texto con el número de monedas
	
	
	// Dibujar el contador de monedas en la esquina superior derecha
	std::string coinText = "Coins: " + std::to_string(player->coinCount);
	Engine::GetInstance().render.get()->DrawText(coinText.c_str(), 40, 10, 155, 50);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		LoadState();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		SaveState();
	
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	// Limpiar la lista de items
	itemList.clear();

	return true;
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

// L15 TODO 1: Implement the Load function
// L15 TODO 2: Implement the Save function
void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Read XML and restore information

	//Player position
	Vector2D playerPos = Vector2D(sceneNode.child("entities").child("player").attribute("x").as_int(),
		sceneNode.child("entities").child("player").attribute("y").as_int());
	player->SetPosition(playerPos);

	//enemies
	// ...

}

// L15 TODO 2: Implement the Save function
void Scene::SaveState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Save info to XML 

	//Player position
	sceneNode.child("entities").child("player").attribute("x").set_value(player->GetPosition().getX());
	sceneNode.child("entities").child("player").attribute("y").set_value(player->GetPosition().getY());

	//enemies
	// ...
	Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Checkpoint.ogg");
	//Saves the modifications to the XML 
	loadFile.save_file("config.xml");
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	//if (control->id == 2 && !gameStarted)  // Start button clicked
	//{
	//	gameStarted = true;

	//	// Initialize game elements now
	//	Start();

	//	// Hide start button
	//	startButton->state = GuiControlState::DISABLED;

	//	return true;
	//}

	// Handle other button events
	LOG("Press Gui Control: %d", control->id);
	return true;
}
