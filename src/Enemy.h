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


private:
    SDL_Texture* texture;
    const char* texturePath;
    int texW, texH;
    pugi::xml_node parameters;
    Animation* currentAnimation = nullptr;
    Animation idle;
    PhysBody* pbody;
    Pathfinding* pathfinding;
    float speed = 3.0f; // Velocidad del enemigo

};
