#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Player.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {
    //initilize textures
    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
    position.setX(parameters.attribute("x").as_int());
    position.setY(parameters.attribute("y").as_int());
    texW = parameters.attribute("w").as_int();
    texH = parameters.attribute("h").as_int();

    //Load animations
    idle.LoadAnimations(parameters.child("animations").child("idle"));
    currentAnimation = &idle;

    // Cargar el efecto de sonido
    coinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/coin.wav");

    pbody = Engine::GetInstance().physics.get()->CreateCircle(
        (int)position.getX() + texH / 2,
        (int)position.getY() + texH / 2,
        texH / 2,
        bodyType::STATIC
    );

    pbody->ctype = ColliderType::ITEM;
    pbody->listener = this;

    if (!parameters.attribute("gravity").as_bool()) {
        pbody->body->SetGravityScale(0);
    }

    return true;
}
bool Item::Update(float dt)
{
    if (!isEnabled) return true;

    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    Engine::GetInstance().render.get()->DrawTexture(
        texture,
        (int)position.getX(),
        (int)position.getY(),
        &currentAnimation->GetCurrentFrame()
    );
    currentAnimation->Update();

    return true;
}
//void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
//    if (physB->ctype == ColliderType::PLAYER && isEnabled) {
//        Engine::GetInstance().audio.get()->PlayFx(coinFxId);
//
//        Player* player = (Player*)physB->listener;
//        if (player != nullptr) {
//            player->coinCount++;
//        }
//
//        isPendingDeletion = true; // Marca para eliminaci�n
//    }
//} 

bool Item::CleanUp() {
    
    return true;
}
void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (physB->ctype == ColliderType::PLAYER && isEnabled) {
        // Reproducir sonido al recoger
        Engine::GetInstance().audio.get()->PlayFx(coinFxId);

        // Acciones espec�ficas al recoger el �tem (incrementar monedas, activar efectos, etc.)
        //Player* player = dynamic_cast<Player*>(Engine::GetInstance().scene.get()->player);
        //if (player != nullptr) {
        //    player->coinCount++; // Incrementar contador de monedas del jugador
        //}

        // Marcar para eliminaci�n
        isEnabled = false;
        isPendingDeletion = true;
    }
}