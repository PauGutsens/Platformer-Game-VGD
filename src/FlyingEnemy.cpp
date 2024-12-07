//#include "Enemy.h"
//#include "Engine.h"
//#include "Textures.h"
//#include "Audio.h"
//#include "Input.h"
//#include "Render.h"
//#include "Scene.h"
//#include "Log.h"
//#include "Physics.h"
//#include "Map.h"
//
//Enemy::Enemy() : Entity(EntityType::ENEMY)
//{
//
//}
//
//Enemy::~Enemy() {
//    delete pathfinding;
//}
//
//bool Enemy::Awake() {
//    return true;
//}
//
//bool Enemy::Start() {
//    // Inicializa el enemigo con una dirección inicial de movimiento hacia adelante (1)
//    direction = true; // El enemigo comienza moviéndose hacia la derecha
//    speed = 2.0f; // Ajusta la velocidad del enemigo
//
//    // Inicialización de la textura y otras configuraciones
//    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
//    position.setX(parameters.attribute("x").as_int());
//    position.setY(parameters.attribute("y").as_int());
//    texW = parameters.attribute("w").as_int();
//    texH = parameters.attribute("h").as_int();
//
//    // Carga las animaciones (idle, etc.)
//    fly.LoadAnimations(parameters.child("animations").child("fly"));
//    currentAnimation = &fly;
//
//    // Crear el cuerpo físico
//    pbody = Engine::GetInstance().physics.get()->CreateCircle(
//        (int)position.getX(),
//        (int)position.getY(),
//        texW / 2,
//        bodyType::DYNAMIC
//    );
//    pbody->listener = this;
//    pbody->ctype = ColliderType::ENEMY;
//
//    // Desactivar gravedad si es necesario
//    
//    pbody->body->SetGravityScale(0);
//
//    return true;
//}
//
//bool Enemy::Update(float dt)
//{
//    bool hasGround = false;
//    b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);
//
//    if (direction == true)
//    {
//        velocity.x = 0, 1 * 16;
//    }
//    else if (direction == false)
//    {
//        velocity.x = -0, 1 * 16;
//    }
//    // Obtén la posición actual del cuerpo físico
//    b2Vec2 pbodyPos = pbody->body->GetPosition();
//
//    // Actualiza la velocidad del cuerpo físico (solo en el eje X, la gravedad controla el eje Y)
//    pbody->body->SetLinearVelocity(b2Vec2(speed * direction, pbody->body->GetLinearVelocity().y));
//    // Dirección del raycast para detectar suelo
//    //Vector2D currentWorldPos = GetPosition();
//    //Vector2D currentTilePos = Engine::GetInstance().map.get()->WorldToMap(currentWorldPos.getX(), currentWorldPos.getY());
//
//    //// Verificar el tile adelante
//    //int nextTileX = direction ? currentTilePos.getX() + 1 : currentTilePos.getX() - 1;
//    //int nextTileY = currentTilePos.getY() + 1; // Un tile abajo, donde debería estar el suelo
//
//    //// Obtener el ID del tile siguiente
//    //int nextTileID = Engine::GetInstance().map.get()->GetTileID(nextTileX, nextTileY);
//
//    //LOG("Current Tile: (%d, %d), Next Tile: (%d, %d), Tile ID: %d",
//    //    currentTilePos.getX(), currentTilePos.getY(),
//    //    nextTileX, nextTileY,
//    //    nextTileID);
//
//    //// Cambiar de dirección si el siguiente tile no es suelo (por ejemplo, ID != suelo)
//    //if (nextTileID != 1) { // Define TILE_GROUND_ID como el ID del suelo
//    //    direction = !direction;
//    //    LOG("No ground ahead. Changing direction to: %s", direction ? "Right" : "Left");
//    //}
//
//    // Ajustar velocidad según la dirección
//
//    velocity.x = direction ? 0.1f * 16 : -0.1f * 16;
//    pbody->body->SetLinearVelocity(velocity);
//    // Actualiza la posición de la textura según la posición física
//    position.setX(METERS_TO_PIXELS(pbodyPos.x) - texW / 2);
//    position.setY(METERS_TO_PIXELS(pbodyPos.y) - texH / 2);
//
//    // Dibuja la textura del enemigo
//    Engine::GetInstance().render.get()->DrawTexture(
//        texture,
//        (int)position.getX(),
//        (int)position.getY(),
//        &currentAnimation->GetCurrentFrame()
//    );
//
//    // Actualiza la animación
//    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
//    currentAnimation->Update();
//
//
//    return true;
//}
//
//bool Enemy::CleanUp()
//{
//    return true;
//}
//
//void Enemy::SetPosition(Vector2D pos) {
//    pos.setX(pos.getX() + texW / 2);
//    pos.setY(pos.getY() + texH / 2);
//    b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
//    pbody->body->SetTransform(bodyPos, 0);
//}
//
//Vector2D Enemy::GetPosition() {
//    b2Vec2 bodyPos = pbody->body->GetTransform().p;
//    Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
//    return pos;
//}
//
//void Enemy::ResetPath() {
//    Vector2D pos = GetPosition();
//    Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
//    pathfinding->ResetPath(tilePos);
//}