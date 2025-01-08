#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

}

Enemy::~Enemy() {
    delete pathfinding;
}

bool Enemy::Awake() {
    return true;
}

bool Enemy::Start() {
    //initilize textures
    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
    position.setX(parameters.attribute("x").as_int());
    position.setY(parameters.attribute("y").as_int());
    texW = parameters.attribute("w").as_int();
    texH = parameters.attribute("h").as_int();

    //Load animations
    idle.LoadAnimations(parameters.child("animations").child("idle"));
    currentAnimation = &idle;

    pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);
    pbody->listener = this;

    //Assign collider type
    pbody->ctype = ColliderType::ENEMY;

    // Set the gravity of the body
    if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);

    // Initialize pathfinding
    pathfinding = new Pathfinding();
    ResetPath();

    return true;
}

bool Enemy::Update(float dt)
{
    
    // Get enemy and player positions
    Vector2D enemyPos = GetPosition();
    Vector2D playerPos = Engine::GetInstance().scene.get()->GetPlayerPosition();

    // Calculate direction to player
    Vector2D direction;
    direction.setX(playerPos.getX() - enemyPos.getX());
    direction.setY(playerPos.getY() - enemyPos.getY());

    // Normalize direction
    float length = sqrt(direction.getX() * direction.getX() + direction.getY() * direction.getY());
    if (length > 0) {
        direction.setX(direction.getX() / length);
        direction.setY(direction.getY() / length);
    }

    // Apply movement using physics
    b2Vec2 velocity;
    velocity.x = direction.getX() * speed;
    velocity.y = direction.getY() * speed;
    pbody->body->SetLinearVelocity(velocity);

    // Update position from physics
    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    // Draw enemy
    Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();

    return true;
}

bool Enemy::CleanUp()
{
    return true;
}

void Enemy::SetPosition(Vector2D pos) {
    pos.setX(pos.getX() + texW / 2);
    pos.setY(pos.getY() + texH / 2);
    b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
    pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
    b2Vec2 bodyPos = pbody->body->GetTransform().p;
    Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
    return pos;
}

void Enemy::ResetPath() {
    Vector2D pos = GetPosition();
    Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
    pathfinding->ResetPath(tilePos);
}
