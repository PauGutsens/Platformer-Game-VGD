#pragma once
#include "Pathfinding.h"

struct SDL_Texture;
enum EnemyState {
	ALIVE,
	DEAD

};

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void ResetPath();

public:
	float speed = 5.0f;

private:


	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, walk;
	PhysBody* pbody;
	Pathfinding* pathfinding;
};

