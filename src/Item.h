#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}
	//void OnCollision(PhysBody* physA, PhysBody* physB);
	void Disable();
	bool isEnabled = true;
	PhysBody* GetPBody() const {
		return pbody;
	}

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	int coinFxId;
	bool isPendingDeletion = false; // Nueva bandera

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;

	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};
