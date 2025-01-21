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
    hasGravity = parameters.attribute("gravity").as_bool(true);

    //Load animations
    walkingRight.LoadAnimations(parameters.child("animations").child("walkingRight"));
    walkingLeft.LoadAnimations(parameters.child("animations").child("walkingLeft"));
    flyingRight.LoadAnimations(parameters.child("animations").child("flyingRight"));
    flyingLeft.LoadAnimations(parameters.child("animations").child("flyingLeft"));

    // Create physics body based on gravity setting
    if (hasGravity) {
        body = Engine::GetInstance().physics.get()->CreateCircle(
            (int)position.getX(),
            (int)position.getY(),
            texW / 2,
            bodyType::DYNAMIC);
        currentAnimation = &walkingRight;
    }
    else {
        body = Engine::GetInstance().physics.get()->CreateCircle(
            (int)position.getX(),
            (int)position.getY(),
            texW / 2,
            bodyType::KINEMATIC);
        body->body->SetGravityScale(0);
        currentAnimation = &flyingRight;
    }

    body->listener = this;
    body->ctype = ColliderType::ENEMY;
    body->body->SetFixedRotation(true);

    // Initialize pathfinding
    pathfinding = new Pathfinding();
    ResetPath();

    return true;
}

bool Enemy::Update(float dt)
{
    if (hasGravity) {
        UpdateWithGravity();
    }
    else {
        UpdateWithoutGravity();
    }

    // Draw enemy
    Engine::GetInstance().render.get()->DrawTexture(texture,
        (int)position.getX(),
        (int)position.getY(),
        &currentAnimation->GetCurrentFrame());
    currentAnimation->Update();
    

    return true;
}

void Enemy::UpdateWithGravity()
{
    Vector2D enemyPos = GetPosition();
    Vector2D playerPos = Engine::GetInstance().scene.get()->GetPlayerPosition();

    float directionX = playerPos.getX() - enemyPos.getX();
    float length = abs(directionX);

    if (length > 0) {
        directionX = directionX / length;
        // Actualizar animación según dirección
        if (directionX > 0) {
            currentAnimation = &walkingRight;
        }
        else {
            currentAnimation = &walkingLeft;
        }
    }

    b2Vec2 velocity = body->body->GetLinearVelocity();
    velocity.x = directionX * speed;

    body->body->SetLinearVelocity(velocity);

    b2Transform pbodyPos = body->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
}

void Enemy::UpdateWithoutGravity()
{
    Vector2D enemyPos = GetPosition();
    Vector2D playerPos = Engine::GetInstance().scene.get()->GetPlayerPosition();

    float directionX = playerPos.getX() - enemyPos.getX();
    float directionY = playerPos.getY() - enemyPos.getY();
    float length = sqrt(directionX * directionX + directionY * directionY);

    if (length > 0) {
        directionX = directionX / length;
        directionY = directionY / length;
        // Actualizar animación según dirección
        if (directionX > 0) {
            currentAnimation = &flyingRight;
        }
        else {
            currentAnimation = &flyingLeft;
        }
    }

    b2Vec2 velocity(directionX * speed, directionY * speed);
    body->body->SetLinearVelocity(velocity);

    b2Transform pbodyPos = body->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
}

bool Enemy::CleanUp() {
    LOG("Cleaning up Enemy resources...");

    // Liberar textura
    if (texture != nullptr) {
        Engine::GetInstance().textures.get()->UnLoad(texture);
        texture = nullptr;
    }

    // Eliminar cuerpo físico
    if (body != nullptr) {
        Engine::GetInstance().physics.get()->DeletePhysBody(body);
        body = nullptr;
    }

    // Liberar pathfinding
    if (pathfinding != nullptr) {
        delete pathfinding;
        pathfinding = nullptr;
    }

    return true;
}

Vector2D Enemy::GetPosition() {
    b2Vec2 bodyPos = body->body->GetTransform().p;
    return Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
}

void Enemy::SetPosition(Vector2D pos) {
    pos.setX(pos.getX() + texW / 2);
    pos.setY(pos.getY() + texH / 2);
    b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
    body->body->SetTransform(bodyPos, 0);
}

void Enemy::ResetPath() {
    Vector2D pos = GetPosition();
    Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
    pathfinding->ResetPath(tilePos);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) 
{
    switch (physB->ctype) {
    case ColliderType::DEATH:
        LOG("Enemy collided with DEATH collider. Removing enemy.");
        Engine::GetInstance().physics.get()->DeletePhysBody(physA); // Eliminar el cuerpo del enemigo
        break;
    default:
        LOG("Enemy collided with another collider.");
        break;
    }
}    

