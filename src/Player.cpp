#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
	
}

Player::~Player() {

}

bool Player::Awake() {

	return true;
}

bool Player::Start() {

	// Inicializar texturas y parámetros
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	// Cargar animaciones
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	walkLeft.LoadAnimations(parameters.child("animation").child("walkLeft")); // Asegúrate de que "walk" esté cargado también

	// Establecer animación inicial
	currentAnimation = &idle;

	// Configuración del cuerpo físico
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	// Configuración de la gravedad
	if (!parameters.attribute("gravity").as_bool())
		pbody->body->SetGravityScale(0);

	// Inicializar efectos de audio
	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	
	IsLookingRight = true;
	IsDashing = false;
	return true;
}


bool Player::Update(float dt)
{
	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	if (!parameters.attribute("gravity").as_bool()) {
		velocity = b2Vec2(0, 9.8);
	}

	IsWalking = false; // Inicializamos el estado como false por defecto
	


	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		if (!IsDashing) {
			velocity.x = 0.2 * 16;
			IsWalking = true;
			IsLookingRight = true;
		}
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		if (!IsDashing) {
			velocity.x = -0.2 * 16;
			IsWalking = true;
			IsLookingRight = false;
		}
	}

	/*LOG("IsLookingRight: %s", IsLookingRight ? "true" : "false");*/

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		IsDashing = true;
		if (IsLookingRight) {
			velocity.x = 0.4f * 16; // Dash speed to the right
		}
		else {
			velocity.x = -0.4f * 16; // Dash speed to the left
		}
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
	{
		IsDashing = false;
	}

	// Cambiar la animación según el estado
	if (IsWalking) {
		if (IsLookingRight = true) {
			currentAnimation = &walk;
		}
		if (IsLookingRight = false) {
			currentAnimation = &walkLeft;
		}
	}
	else {
		currentAnimation = &idle;
	}

	//Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isJumping) {
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumping, we don't want to apply gravity, we use the current velocity produced by the jump
	if (isJumping) {
		float verticalVelocity = pbody->body->GetLinearVelocity().y;

		// Allow landing when vertical velocity is near zero
		if (verticalVelocity > -0.1f && verticalVelocity < 0.1f) {
			isJumping = false;
		}
		else {
			velocity.y = verticalVelocity;
		}
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	// Dibujar la textura con la animación actual
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
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		Engine::GetInstance().audio.get()->PlayFx(pickCoinFxId);
		Engine::GetInstance().physics.get()->DeletePhysBody(physB); // Deletes the body of the item from the physics world
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::ENEMY:
		if (IsDashing)
		{
			Engine::GetInstance().physics.get()->DeletePhysBody(physB);
		}
		else {
			/*Engine::GetInstance().scene.get()->LoadState();*/
			LOG("YOU DIED");
		}
		break;
	case ColliderType::DEATH:
		
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
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Player::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}
