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
	titleScreen = new TitleScreen();  // Inicializar aquí
	gameStarted = false;
}

Scene::~Scene()
{
	CleanUp();
}

// Called before render is available
bool Scene::Awake()
{
	
	LOG("Loading Scene");
	bool ret = true;

	if (!gameStarted && titleScreen != nullptr)
	{
		titleScreen->Awake();  // Si TitleScreen tiene función Awake
	}

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
		//player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
		//player->SetParameters(configParameters.child("entities").child("player"));

		//// Crear items
		//pugi::xml_node items = configParameters.child("entities").child("Items");
		//for (pugi::xml_node itemNode = items.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
		//{
		//	Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
		//	item->SetParameters(itemNode);
		//	itemList.push_back(item);  // Añadir a nuestra lista
		//}

		// Create enemies
		/*for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy");
			enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
			enemy->SetParameters(enemyNode);
			enemyList.push_back(enemy);
		}*/
		// L16: TODO 2: Instantiate a new GuiControlButton in the Scene
		/*SDL_Rect btPos = { 520, 350, 120,20 };
		guiBt = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Menu", btPos, this);*/

		return ret;
	/*}*/
}

// Called before the first frame
bool Scene::Start()
{
	if (!gameStarted) {
		if (titleScreen == nullptr) {
			LOG("Error: titleScreen is null!");
			return false;
		}
		return titleScreen->Start();
	}

	// Todo el resto de la inicialización del juego se moverá a una nueva función
	return InitGame();
}
// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	if (!gameStarted) {
		titleScreen->Update(dt);
		if (titleScreen->ShouldTransitionToGame()) {
			gameStarted = true;
			titleScreen->CleanUp();
			delete titleScreen;
			titleScreen = nullptr;
			return InitGame();  // Inicializa el juego cuando se presiona el botón
		}
		return true;
	}
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
		/*if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_DOWN) {
			enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
			enemyList[0]->ResetPath();
		}*/
	
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
bool Scene::CleanUp() {
	LOG("Cleaning up Scene...");

	// Limpiar TitleScreen si aún no se ha iniciado el juego
	if (!gameStarted && titleScreen != nullptr) {
		LOG("Cleaning up TitleScreen...");
		titleScreen->CleanUp();
		delete titleScreen;
		titleScreen = nullptr;
	}

	try {
		// Limpiar lista de items
		for (auto* item : itemList) {
			if (item != nullptr) {
				delete item;
			}
		}
		itemList.clear();

		// Limpiar lista de enemigos
		for (auto* enemy : enemyList) {
			if (enemy != nullptr) {
				enemy->CleanUp();
				delete enemy;
			}
		}
		enemyList.clear();

		// Liberar texturas
		if (mouseTileTex != nullptr) {
			Engine::GetInstance().textures.get()->UnLoad(mouseTileTex);
			mouseTileTex = nullptr;
		}
		if (coinIconTexture != nullptr) {
			Engine::GetInstance().textures.get()->UnLoad(coinIconTexture);
			coinIconTexture = nullptr;
		}

		return true;
	}
	catch (const std::exception& e) {
		LOG("Exception during Scene::CleanUp: %s", e.what());
		return false;
	}
}
// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

// L15 TODO 1: Implement the Load function
// L15 TODO 2: Implement the Save function
void Scene::LoadState() {
	LOG("Loading state...");

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (!result) {
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	// Cargar posición del jugador
	pugi::xml_node playerNode = sceneNode.child("entities").child("player");
	if (playerNode) {
		Vector2D playerPos = Vector2D(
			playerNode.attribute("x").as_int(),
			playerNode.attribute("y").as_int()
		);

		if (player != nullptr) {
			player->SetPosition(playerPos);
		}
		else {
			LOG("Player is null during LoadState. Initializing a new player...");
			player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
			if (player != nullptr) {
				player->SetParameters(playerNode);
				player->Start();
			}
			else {
				LOG("Failed to initialize player during LoadState.");
			}
		}
	}
	else {
		LOG("Player node not found in XML during LoadState.");
	}

	//// Cargar enemigos
	//LOG("Clearing existing enemies...");
	//for (auto* enemy : enemyList) {
	//	if (enemy != nullptr) {
	//		enemy->CleanUp();
	//		delete enemy;
	//	}
	//}
	//enemyList.clear();

	//LOG("Loading enemies...");
	//pugi::xml_node enemiesNode = sceneNode.child("entities").child("enemies");
	//for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {
	//	Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
	//	if (enemy == nullptr) {
	//		LOG("Failed to create enemy during LoadState!");
	//		continue;
	//	}

	//	// Configurar posición del enemigo
	//	if (enemyNode.attribute("x") && enemyNode.attribute("y")) {
	//		enemy->SetPosition(Vector2D(
	//			enemyNode.attribute("x").as_int(),
	//			enemyNode.attribute("y").as_int()
	//		));
	//	}
	//	else {
	//		LOG("Enemy node is missing position attributes! Skipping...");
	//		delete enemy;
	//		continue;
	//	}

	//	// Configurar velocidad (si existe en el XML)
	//	if (enemyNode.attribute("speed")) {
	//		enemy->SetSpeed(enemyNode.attribute("speed").as_float());
	//	}
	//	else {
	//		LOG("Enemy node is missing speed attribute! Setting default speed.");
	//		enemy->SetSpeed(2.0f); // Velocidad predeterminada
	//	}

	//	// Inicializar el enemigo
	//	if (!enemy->Start()) {
	//		LOG("Failed to initialize enemy during LoadState! Deleting...");
	//		delete enemy;
	//		continue;
	//	}

	//	enemyList.push_back(enemy);
	//	LOG("Enemy loaded and initialized at position (%d, %d)",
	//		enemyNode.attribute("x").as_int(), enemyNode.attribute("y").as_int());
	//}

	LOG("State loaded successfully.");
}
// L15 TODO 2: Implement the Save function
void Scene::SaveState() {
	LOG("Saving state...");

	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	if (!result) {
		LOG("Could not load file for saving. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = saveFile.child("config").child("scene");

	// Guardar posición del jugador
	sceneNode.child("entities").child("player").attribute("x").set_value(player->GetPosition().getX());
	sceneNode.child("entities").child("player").attribute("y").set_value(player->GetPosition().getY());

	// Guardar enemigos
	pugi::xml_node enemiesNode = sceneNode.child("entities").child("enemies");
	enemiesNode.remove_children();  // Eliminar nodos existentes para evitar duplicados

	for (auto* enemy : enemyList) {
		if (enemy == nullptr) continue;

		pugi::xml_node enemyNode = enemiesNode.append_child("enemy");
		enemyNode.append_attribute("x") = enemy->GetPosition().getX();
		enemyNode.append_attribute("y") = enemy->GetPosition().getY();
		enemyNode.append_attribute("speed") = enemy->GetSpeed();
	}

	// Guardar el archivo modificado
	if (!saveFile.save_file("config.xml")) {
		LOG("Failed to save state to file!");
	}
	else {
		LOG("State saved successfully.");
	}
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
bool Scene::InitGame()
{
	// 1. Asegurarnos que el sistema de física está activo
	/*if (!Engine::GetInstance().physics->IsEnabled()) {
		if (!Engine::GetInstance().physics->Enable()) {
			LOG("Failed to enable physics system!");
			return false;
		}
	}*/

	// 2. Cargar el mapa primero ya que otros elementos pueden depender de él
	if (!Engine::GetInstance().map->Load(
		configParameters.child("map").attribute("path").as_string(),
		configParameters.child("map").attribute("name").as_string()))
	{
		LOG("Failed to load map!");
		return false;
	}

	// 3. Crear y inicializar el jugador
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	if (!player) {
		LOG("Failed to create player!");
		return false;
	}
	player->SetParameters(configParameters.child("entities").child("player"));
	if (!player->Start()) {
		LOG("Failed to initialize player!");
		return false;
	}

	// 4. Crear e inicializar enemigos
	pugi::xml_node enemiesNode = configParameters.child("entities").child("enemies");
	if (enemiesNode) {
		for (pugi::xml_node enemyNode = enemiesNode.child("enemy");
			enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
			if (!enemy) {
				LOG("Failed to create enemy!");
				continue;
			}

			// Configurar el enemigo antes de inicializarlo
			enemy->SetParameters(enemyNode);

			// Inicializar el enemigo
			if (!enemy->Awake()) {
				LOG("Failed to awake enemy!");
				delete enemy;
				continue;
			}

			if (!enemy->Start()) {
				LOG("Failed to initialize enemy!");
				delete enemy;
				continue;
			}

			enemyList.push_back(enemy);
			LOG("Enemy created and initialized successfully");
		}
	}

	// 5. Crear e inicializar items
	pugi::xml_node items = configParameters.child("entities").child("Items");
	if (items) {
		for (pugi::xml_node itemNode = items.child("item");
			itemNode; itemNode = itemNode.next_sibling("item"))
		{
			Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
			if (!item) {
				LOG("Failed to create item!");
				continue;
			}

			item->SetParameters(itemNode);
			if (!item->Start()) {
				LOG("Failed to initialize item!");
				delete item;
				continue;
			}

			itemList.push_back(item);
		}
	}

	// 6. Cargar recursos adicionales
	mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");
	if (!mouseTileTex) {
		LOG("Failed to load mouse tile texture!");
	}

	coinIconTexture = Engine::GetInstance().textures.get()->Load("Assets/Textures/goldCoin.png");
	if (!coinIconTexture) {
		LOG("Failed to load coin icon texture!");
	}

	// 7. Configurar cámara
	int w, h;
	Engine::GetInstance().window.get()->GetWindowSize(w, h);
	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	// 8. Iniciar música
	Engine::GetInstance().audio.get()->PlayMusic("Assets/Textures/Audio/Music/game-music-loop-3-144252.ogg", 0.1f);

	return true;
}