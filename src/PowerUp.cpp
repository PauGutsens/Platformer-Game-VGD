#include "PowerUp.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Player.h"

PowerUp::PowerUp() : Entity(EntityType::POWERUP)
{
	name = "powerup";
}

PowerUp::~PowerUp() {}

bool PowerUp::Awake() {
	return true;
}

bool PowerUp::Start() {
    //initilize textures
    texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
    position.setX(parameters.attribute("x").as_int());
    position.setY(parameters.attribute("y").as_int());
    texW = parameters.attribute("w").as_int();
    texH = parameters.attribute("h").as_int();

    pbody = Engine::GetInstance().physics.get()->CreateCircle(
        (int)position.getX() + texH / 2,
        (int)position.getY() + texH / 2,
        texH / 2,
        bodyType::STATIC
    );

    pbody->ctype = ColliderType::POWERUP;
    pbody->listener = this;

    if (!parameters.attribute("gravity").as_bool()) {
        pbody->body->SetGravityScale(0);
    }

    return true;
}
bool PowerUp::Update(float dt)
{
    if (!isEnabled) return true;

    b2Transform pbodyPos = pbody->body->GetTransform();
    position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
    position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

    Engine::GetInstance().render.get()->DrawTexture(
        texture,
        (int)position.getX(),
        (int)position.getY());

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
//        isPendingDeletion = true; // Marca para eliminación
//    }
//} 

bool PowerUp::CleanUp() {
    
    return true;
}
void PowerUp::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (physB->ctype == ColliderType::PLAYER && isEnabled) {
        // Reproducir sonido al recoger

        // Acciones específicas al recoger el ítem (incrementar monedas, activar efectos, etc.)
        //Player* player = dynamic_cast<Player*>(Engine::GetInstance().scene.get()->player);
        //if (player != nullptr) {
        //    player->coinCount++; // Incrementar contador de monedas del jugador
        //}

        // Marcar para eliminación
        isEnabled = false;
        isPendingDeletion = true;
    }
}