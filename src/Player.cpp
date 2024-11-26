#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(96, 96);
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	currentAnimation = &walk;
	

	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), (texW / 2) - 1, bodyType::DYNAMIC);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	// Obtener la velocidad inicial del cuerpo físico
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	// Variable para determinar si el jugador está en movimiento horizontal
	bool isWalking = false;

	// Movimiento hacia la izquierda
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2 * 16; // Velocidad hacia la izquierda
		isWalking = true;      // El jugador está caminando
	}

	// Movimiento hacia la derecha
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2 * 16; // Velocidad hacia la derecha
		isWalking = true;      // El jugador está caminando
	}

	// Saltar
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping) {
		// Impulso inicial hacia arriba
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce + 0.6), true);
		isJumping = true;
	}

	// Mantener la velocidad vertical actual si está saltando
	if (isJumping) {
		velocity.y = pbody->body->GetLinearVelocity().y;
	}

	// Aplicar la velocidad calculada al cuerpo físico
	pbody->body->SetLinearVelocity(velocity);

	// Cambiar la animación según el estado
	if (isWalking && currentAnimation != &walk) {
		currentAnimation = &walk; // Activar animación de caminar
	}
	else if (!isWalking && currentAnimation != &idle) {
		currentAnimation = &idle; // Activar animación de estar quieto
	}

	// Actualizar la posición del jugador basada en el cuerpo físico
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	// Dibujar la textura y actualizar la animación
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}



bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//reset the jump flag when touching the ground
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("End Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		Engine::GetInstance().audio.get()->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}
