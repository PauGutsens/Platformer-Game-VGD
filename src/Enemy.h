#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:
    Enemy();
    virtual ~Enemy();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool CleanUp();
    void OnCollision(PhysBody* physA, PhysBody* physB);
    void SetParameters(pugi::xml_node parameters) {
        this->parameters = parameters;
    }

    void SetPosition(Vector2D pos);
    Vector2D GetPosition();
    void ResetPath();
    void SetSpeed(float s) {
        speed = s;
    }

    float GetSpeed() const {
        return speed;
    }

        bool enemyDead = false;
        PhysBody* body;  // Solo un cuerpo físico

    bool IsOnScreen();


private:
    void UpdateWithGravity();
    void UpdateWithoutGravity();

private:
    SDL_Texture* texture;
    const char* texturePath;
    int texW, texH;
    pugi::xml_node parameters;
    Animation* currentAnimation = nullptr;
    Animation walkingLeft, walkingRight, flyingLeft, flyingRight;
    Pathfinding* pathfinding;
    float speed = 3.0f;
    bool hasGravity;
};