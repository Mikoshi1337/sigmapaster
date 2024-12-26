#include "defs.h"
#include "HitSounds.h"
#include "AntiHack.h"
#include "Math/weapons.h"
#include "Cheat\Visuals\menu\Elements\norifications.h"
Vector3 last_penis_pos = Vector3::Zero();
int flick = 0;
int yaw = 100;

bool targeted = false;
float target_time = 0.f;
inline float set_distance = 0.f;

using namespace hk_defs;

void SimulateProjectile(Vector3 position, Vector3 velocity, float partialTime, float travelTime, Vector3 gravity, float drag, Vector3 prevPosition, Vector3 prevVelocity)
{
	float num = 0.03125f;
	prevPosition = position;
	prevVelocity = velocity;
	if (partialTime > 0)
	{
		float num2 = num - partialTime;
		if (travelTime < num2)
		{
			prevPosition = position;
			prevVelocity = velocity;
			position += velocity * travelTime;
			partialTime += travelTime;
			return;
		}
		prevPosition = position;
		prevVelocity = velocity;
		position += velocity * num2;
		velocity += gravity * num;
		velocity -= velocity * (drag * num);
		travelTime -= num2;
	}
	int num3 = int(travelTime / num);
	for (int i = 0; i < num3; i++)
	{
		prevPosition = position;
		prevVelocity = velocity;
		position += velocity * num;
		velocity += gravity * num;
		velocity -= velocity * (drag * num);
	}
	partialTime = travelTime - num * (float)num3;
	if (partialTime > 0)
	{
		prevPosition = position;
		prevVelocity = velocity;
		position += velocity * partialTime;
	}
}

bool BulletTP(Projectile* instance, Vector3 NextCurrentPosition, Vector3 CurrentPosition, Vector3 CurrentVelocity, float deltaTime)
{
	Vector3 aa; Vector3 bb;

	BasePlayer* targetPlayer = target_player;
	auto playerProjectileUpdate = (PlayerProjectileUpdate*)methods::object_new(il2cpp::_init_class(("PlayerProjectileUpdate"), ("ProtoBuf")));

	if (instance->projectileID() == 0)
		return false;

	if (instance->integrity() <= 0.f)
		return false;

	if (!targetPlayer)
		return false;

	auto transform = targetPlayer->transform_(head);
	auto targetPos = transform->position();

	auto Line = NextCurrentPosition - CurrentPosition;
	auto LineLength = Line.UnityMagnitude();
	Line.UnityNormalize_Sero();

	auto CenterPosition = targetPlayer->PivotPoint() + Vector3(0.f, 0.9f, 0.f);
	auto v = CenterPosition - CurrentPosition;
	auto d = v.DotSero(Line);

	if (d < 0.0f)
	{
		d = 0.0f;
	}
	else if (d > LineLength)
	{
		d = LineLength;
	}

	auto OriginalClosestPointOnLine = CurrentPosition + Line * d;
	auto ClosestPointOnLine = OriginalClosestPointOnLine;

	auto ClosestPoint = utils::ClosestPoint(targetPlayer, ClosestPointOnLine);
	auto OriginalDistance = targetPlayer->Distance(ClosestPointOnLine);
	auto Distance = OriginalDistance;

	Vector3 HitPointWorld = ClosestPoint;

	Vector3 Hitbox = targetPlayer->transform()->position() + Vector3(0, 1.49f, 0);
	Vector3 Dir_ = Hitbox - instance->currentPosition();

	if (VarSs::combat::bullet_tp_type == 0)
	{
		if (Distance > 1.8f)
		{
			return false;
		}
	}

	if (VarSs::combat::bullet_tp_type == 1)
	{
		if (Distance > 2.2f)
		{
			return false;
		}
	}

	if (VarSs::combat::bullet_tp_type == 2)
	{
		if (Distance > 2.5f)
		{
			return false;
		}
	}

	if (VarSs::combat::ExtentForMoutend)
	{
		if (target_player->in_minicopter())
		{
			Distance > 10.f;
			return false;
		}
	}

	if (Distance > 1.2f)
	{
		auto endPositionTrajectoryUsage = Distance - 1.2f + 0.01f;
		auto amountNeeded = endPositionTrajectoryUsage / Distance;
		auto direction = HitPointWorld - ClosestPointOnLine;
		auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

		if (ClosestPointOnLine.Distance(newPosition) > 1.f)
			return false;

		ClosestPointOnLine = newPosition;
		Distance = targetPlayer->Distance(ClosestPointOnLine);
	}

	if (Distance > 1.f)
	{
		auto playerDistanceUsage = min(0.19f, Distance - 1.f);
		auto amountNeeded = 1.f - playerDistanceUsage / Distance;
		auto direction = HitPointWorld - ClosestPointOnLine;
		auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

		HitPointWorld = newPosition;
	}

	instance->previousPosition() = instance->currentPosition();
	instance->currentPosition() = OriginalClosestPointOnLine;
	instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
	instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

	if (playerProjectileUpdate)
	{
		((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();

		LocalPlayer::Entity()->SendProjectileUpdate(playerProjectileUpdate);

		playerProjectileUpdate->curPosition() = instance->currentPosition() + (Dir_.Normalized() * 0.97f);

		LocalPlayer::Entity()->SendProjectileUpdate(playerProjectileUpdate);
	}

	instance->previousPosition() = instance->currentPosition();
	instance->currentPosition() = ClosestPointOnLine;
	instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
	instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

	if (playerProjectileUpdate)
	{
		((PlayerProjectileUpdate*)playerProjectileUpdate)->projectileID() = instance->projectileID();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->curPosition() = instance->currentPosition();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->curVelocity() = instance->currentVelocity();
		((PlayerProjectileUpdate*)playerProjectileUpdate)->travelTime() = instance->traveledTime();

		LocalPlayer::Entity()->SendProjectileUpdate(playerProjectileUpdate);

		playerProjectileUpdate->curPosition() = instance->currentPosition() + (Dir_.Normalized() * 0.97f);

		LocalPlayer::Entity()->SendProjectileUpdate(playerProjectileUpdate);
	}

	HitTest* hTest = instance->hitTest();

	instance->transform()->set_position(HitPointWorld);

	SimulateProjectile(instance->currentPosition(), instance->currentVelocity(), instance->tumbleSpeed(), max(instance->traveledTime() - instance->closeFlybyDistance(), 0), Physics::get_gravity() * instance->gravityModifier(), instance->drag(), aa, bb);

	Ray r(instance->transform()->position(), HitPointWorld - instance->currentPosition());

	hTest->DidHit() = true;
	hTest->HitEntity() = target_player;
	hTest->HitTransform() = transform;
	hTest->HitPoint() = transform->InverseTransformPoint(HitPointWorld);
	hTest->AttackRay() = r;
	hTest->Radius() = 0.00f;
	instance->DoHit(hTest, hTest->HitPointWorld(), hTest->HitNormalWorld());
	instance->Update();

	return true;
}

namespace hk {
	namespace exploit {
		void ProjectileUpdate_hk(Projectile* projectile) {

			if (VarSs::combat::instanthit)
			{
				projectile->initialDistance() = 0.f;
			}

			return original_Update(projectile);
		}

		void DoMovement(Projectile* projectile, float deltaTime) {

			if (VarSs::combat::bullet_tp && projectile->isAuthoritative() && projectile->isAlive()) {
				auto transform = reinterpret_cast<BasePlayer*>(target_player)->transform_(head);
				if (transform) {
					Vector3 a = projectile->currentVelocity() * deltaTime;
					float magnitude = a.Length();
					float num2 = 1 / magnitude;
					Vector3 vec2 = a * num2;
					Vector3 vec3 = projectile->currentPosition() + vec2 * magnitude;

					BulletTP(projectile, vec3, projectile->currentPosition(), projectile->currentVelocity(), deltaTime);
				}
			}

			BaseProjectile* held = held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			auto currentVelocity = projectile->currentVelocity() * deltaTime;
			auto intmag = currentVelocity.Magnitude();
			auto intnum = 1.f / intmag;
			auto speed = currentVelocity * intnum;
			auto lineposition = projectile->currentPosition() + speed * currentVelocity.Magnitude();
			auto ringsition = projectile->currentPosition() + speed * currentVelocity.Magnitude();

			static float Alpha = 2500;
			static bool Tick = false;
			static float Speed = 2.5f;

			if (!Tick || Alpha != 2100)
			{
				Tick = false;
				if (!(Alpha >= 2500))
					Alpha += Speed;
				else if (Alpha >= 2500)
					Tick ^= 1;
				if (Alpha >= 2100)
				{
					Alpha = -300;
				}
			}

			const char* _text;
			static float r = 0.50f, g = 0.00f, b = 0.50f, a = 255.f;
			static int cases = 0;
			switch (cases)
			{
			case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
			case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
			case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
			case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			}

			if (projectile->isAuthoritative() && VarSs::debug::bulletline) {

				dd1aw::Line(projectile->currentPosition(), lineposition, Color(VarSs::colors::BulletLine.x, VarSs::colors::BulletLine.y, VarSs::colors::BulletLine.z, 1), .9f, false, true);
			}

			if (projectile->isAuthoritative() && VarSs::debug::bulletline2) {
				int col = rand() % 100;

				if (col > 90)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(254, 127, 127, 255), 2.5f, false, true);
				else if (col < 90 && col > 80)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(255, 128, 240, 255), 2.5f, false, true);
				else if (col < 80 && col > 70)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(174, 128, 255, 255), 2.5f, false, true);
				else if (col < 70 && col > 60)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(128, 148, 255, 255), 2.5f, false, true);
				else if (col < 60 && col > 50)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(128, 205, 255, 255), 2.5f, false, true);
				else if (col < 50 && col > 40)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(128, 255, 244, 255), 2.5f, false, true);
				else if (col < 40 && col > 30)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(128, 255, 152, 255), 2.5f, false, true);
				else if (col < 30 && col > 20)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(214, 255, 128, 255), 2.5f, false, true);
				else if (col < 20 && col > 10)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(255, 223, 128, 255), 2.5f, false, true);
				else if (col < 10)
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(255, 128, 128, 255), 2.5f, false, true);
				else
					dd1aw::Line(projectile->currentPosition(), lineposition, Color(255, 255, 255, 255), 2.5f, false, true);
			}
			return original_domovement(projectile, deltaTime);
		}
	}
	namespace misc {
		float LastUpdate = 0.f;

		void __fastcall SetFlying(ModelState* a1, bool a2) { }
		void SendClientTick(BasePlayer* baseplayer) {

			if (!baseplayer) return baseplayer->SendClientTick();
				if (Time::fixedTime() > (target_time + 0.1f) && target_time != 0.0f && targeted)
				{
					target_time = 0.0f;
					targeted = false;
				}
				
			if (baseplayer->userID() == LocalPlayer::Entity()->userID()) {
				Vector3 current = baseplayer->transform()->position();
				Vector3 old = baseplayer->lastSentTick()->position();
				Vector3 vector4 = (baseplayer->transform()->position() - baseplayer->lastSentTick()->position());
				Vector3 overrided = Vector3(current.x, current.y, current.z);

				if (VarSs::combat::ZOZO && GetAsyncKeyState(VarSs::keys::ZOZO)) {
					
					BaseProjectile* ent = baseplayer->GetHeldEntity<BaseProjectile>();
					LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Mounted);
					ent->_ServerRPC(("RPC_LootPlayer"));
				}
				if (VarSs::misc::fly_auto_stopper && VFlyhack >= (VMaxFlyhack - 1.5f))
				{
					overrided = Vector3(overrided.x, current.y < old.y ? (current.y - 0.3f) : old.y, overrided.z);
				}
				if (VarSs::misc::fly_auto_stopper && HFlyhack >= (HMaxFlyhack - 2.7f))
				{
					overrided = Vector3(old.x, overrided.y, old.z);
				}
				if (VarSs::misc::fly_auto_stopper && HFlyhack >= (HMaxFlyhack - 2.7f) ||
					VarSs::misc::fly_auto_stopper && VFlyhack >= (VMaxFlyhack - 1.5f))
				{
					if (overrided != current)
						baseplayer->movement()->TeleportTo(overrided);
				}
			}
			return baseplayer->SendClientTick();
		}

		//void UnloadChams() {
		//	/* Reload Chams */
		//	{				
		//		if (Gold)
		//		{
		//			Gold = nullptr;
		//		}

		//		if (Fade)
		//		{
		//			Fade = nullptr;
		//		}

		//		if (Damascus)
		//		{
		//			Damascus = nullptr;
		//		}

		//		if (DamascusMat)
		//		{
		//			DamascusMat = nullptr;
		//		}

		//		if (DamascusMat)
		//		{
		//			DamascusMat = nullptr;
		//		}

		//		if (Fire)
		//		{
		//			Fire = nullptr;
		//		}				

		//		if (Pixel)
		//		{
		//			Pixel = nullptr;
		//		}

		//		if (Ghost)
		//		{
		//			Ghost = nullptr;
		//		}

		//		if (Lighting)
		//		{
		//			Lighting = nullptr;
		//		}

		//		if (DesolveMat)
		//		{
		//			DesolveMat = nullptr;
		//		}

		//		if (Flat)
		//		{
		//			Flat = nullptr;
		//		}

		//		if (RainbowFrame)
		//		{
		//			RainbowFrame = nullptr;
		//		}

		//		if (GalaxyFire)
		//		{
		//			GalaxyFire = nullptr;
		//		}

		//		if (GalaxyNightFire)
		//		{
		//			GalaxyNightFire = nullptr;
		//		}
		//	}
		//}	


		void HandChams() {
			if (!LocalPlayer::Entity()->is_alive() || LocalPlayer::Entity()->is_sleeping()) return;

			if (!VarSs::visuals::hand_chams) return;

			static float r = 1.00f, g = 0.00f, b = 1.00f;
			static int cases = 0;
			switch (cases) {
			case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
			case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
			case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
			case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			}

			BaseViewModel::ActiveModel()->useViewModelCamera() = false;

			auto renderers = BaseViewModel::ActiveModel()->GetComponentsInChildren(Type::Renderer());
			if (renderers) {
				for (int i = 0; i < renderers->size(); i++) {
					Renderer_* renderer = reinterpret_cast<Renderer_*>(renderers->get(i));
					if (!renderer || !renderer->material()) continue;

					Material* material = renderer->material();
					if (!material) continue;

					switch (VarSs::visuals::chams_render_mode) {
					case 0: {
						if (!material->get_name()->Contains(StringIl::New("fumes")) && !material->get_name()->Contains(StringIl::New("flash")) && !material->get_name()->Contains(StringIl::New("holosight"))) {
							if (VarSs::visuals::hand_chams_type == 0) {
								renderer->set_material(Gold);
							}
							else if (VarSs::visuals::hand_chams_type == 1) {
								renderer->set_material(Fade);
							}
							else if (VarSs::visuals::hand_chams_type == 2) {
								renderer->set_material(Frame);
							}
							else if (VarSs::visuals::hand_chams_type == 3) {
								renderer->set_material(Rainbow);
							}
							else if (VarSs::visuals::hand_chams_type == 4) {
								renderer->set_material(Ghost);
							}
							else if (VarSs::visuals::hand_chams_type == 5) {
								renderer->set_material(Glitch);
							}
							else if (VarSs::visuals::hand_chams_type == 6) {
								renderer->set_material(Glitch2);
							}
							else if (VarSs::visuals::hand_chams_type == 7) {
								renderer->set_material(Glitch3);
							}
							else if (VarSs::visuals::hand_chams_type == 8) {
								renderer->set_material(Glitch4);
							}
							else if (VarSs::visuals::hand_chams_type == 9) {
								renderer->set_material(Galaxy);
							}
							else if (VarSs::visuals::hand_chams_type == 10) {
								renderer->set_material(Galaxy2);
							}
							else if (VarSs::visuals::hand_chams_type == 11) {
								renderer->set_material(Galaxy3);
							}
							else if (VarSs::visuals::hand_chams_type == 12) {
								renderer->set_material(Galaxy4);
							}
							else if (VarSs::visuals::hand_chams_type == 13) {
								renderer->set_material(Galaxy4);
							}
						}

						break;

					}
					case 1: {
						if (!material->get_name()->Contains(StringIl::New("fumes")) && !material->get_name()->Contains(StringIl::New("flash")) && !material->get_name()->Contains(StringIl::New("holosight"))) {
							if (material->get_name()->Contains(StringIl::New("Arm")) || material->get_name()->Contains(StringIl::New("hoodie")) || material->get_name()->Contains(StringIl::New("Glove"))
								|| material->get_name()->Contains(StringIl::New("Hand")) || material->get_name()->Contains(StringIl::New("Suit")) || material->get_name()->Contains(StringIl::New("tshirt"))
								|| material->get_name()->Contains(StringIl::New("hazmat")) || material->get_name()->Contains(StringIl::New("torso"))) {
								if (VarSs::visuals::hand_chams_type == 0) {
									renderer->set_material(Gold);
								}
								else if (VarSs::visuals::hand_chams_type == 1) {
									renderer->set_material(Fade);
								}
								else if (VarSs::visuals::hand_chams_type == 2) {
									renderer->set_material(Frame);
								}
								else if (VarSs::visuals::hand_chams_type == 3) {
									renderer->set_material(Rainbow);
								}
								else if (VarSs::visuals::hand_chams_type == 4) {
									renderer->set_material(Ghost);
								}
								else if (VarSs::visuals::hand_chams_type == 5) {
									renderer->set_material(Glitch);
								}
								else if (VarSs::visuals::hand_chams_type == 6) {
									renderer->set_material(Glitch2);
								}
								else if (VarSs::visuals::hand_chams_type == 7) {
									renderer->set_material(Glitch3);
								}
								else if (VarSs::visuals::hand_chams_type == 8) {
									renderer->set_material(Glitch4);
								}
								else if (VarSs::visuals::hand_chams_type == 9) {
									renderer->set_material(Galaxy);
								}
								else if (VarSs::visuals::hand_chams_type == 10) {
									renderer->set_material(Galaxy2);
								}
								else if (VarSs::visuals::hand_chams_type == 11) {
									renderer->set_material(Galaxy3);
								}
								else if (VarSs::visuals::hand_chams_type == 12) {
									renderer->set_material(Galaxy4);
								}
								else if (VarSs::visuals::hand_chams_type == 13) {
									renderer->set_material(Galaxy4);
								}
							}
						}

						break;

					}
					case 2: {
						if (!material->get_name()->Contains(StringIl::New("fumes")) && !material->get_name()->Contains(StringIl::New("flash")) && !material->get_name()->Contains(StringIl::New("holosight"))) {
							if (!material->get_name()->Contains(StringIl::New("Arm")) && !material->get_name()->Contains(StringIl::New("hoodie")) && !material->get_name()->Contains(StringIl::New("Glove"))
								&& !material->get_name()->Contains(StringIl::New("Hand")) && !material->get_name()->Contains(StringIl::New("Suit")) && !material->get_name()->Contains(StringIl::New("tshirt"))
								&& !material->get_name()->Contains(StringIl::New("hazmat")) && !material->get_name()->Contains(StringIl::New("torso"))) {
								if (VarSs::visuals::hand_chams_type == 0) {
									renderer->set_material(Gold);
								}
								else if (VarSs::visuals::hand_chams_type == 1) {
									renderer->set_material(Fade);
								}
								else if (VarSs::visuals::hand_chams_type == 2) {
									renderer->set_material(Frame);
								}
								else if (VarSs::visuals::hand_chams_type == 3) {
									renderer->set_material(Rainbow);
								}
								else if (VarSs::visuals::hand_chams_type == 4) {
									renderer->set_material(Ghost);
								}
								else if (VarSs::visuals::hand_chams_type == 5) {
									renderer->set_material(Glitch);
								}
								else if (VarSs::visuals::hand_chams_type == 6) {
									renderer->set_material(Glitch2);
								}
								else if (VarSs::visuals::hand_chams_type == 7) {
									renderer->set_material(Glitch3);
								}
								else if (VarSs::visuals::hand_chams_type == 8) {
									renderer->set_material(Glitch4);
								}
								else if (VarSs::visuals::hand_chams_type == 9) {
									renderer->set_material(Galaxy);
								}
								else if (VarSs::visuals::hand_chams_type == 10) {
									renderer->set_material(Galaxy2);
								}
								else if (VarSs::visuals::hand_chams_type == 11) {
									renderer->set_material(Galaxy3);
								}
								else if (VarSs::visuals::hand_chams_type == 12) {
									renderer->set_material(Galaxy4);
								}
								else if (VarSs::visuals::hand_chams_type == 13) {
									renderer->set_material(Galaxy4);
								}
							}
						}

						break;

					}
					}
				}
			}
		}
		void PowerShoot(int shotsLeft, BaseProjectile* held, Vector3 target, BasePlayer* plly) {
			float m = held->repeatDelay() * VarSs::combat::insta_kill_value;
			float timeSinceLastTick = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime());
			int r = timeSinceLastTick / m;
			float lastshottime;

			if (VarSs::combat::instakill && r > 1 && shotsLeft > 0) {
				float NextShot = lastshottime + held->repeatDelay() + 0.2;
				if (Time::fixedTime() > NextShot) {
					if (utils::LineOfSight(target, plly->eyes()->position())) {
						held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
						held->DoAttack();
						held->LaunchProjectile();
						held->primaryMagazine()->contents()--;
						held->UpdateAmmoDisplay();
						held->ShotFired();
						held->DidAttackClientside();
						held->BeginCycle();
					}

					if (shotsLeft > 1) {
						PowerShoot(shotsLeft - 1, held, target, plly);
					}
					else {
						lastshottime = Time::fixedTime();
					}
				}
			}
		}
		void ClientInput(/*Weather* weather,*/BasePlayer* plly, ModelState* ModelState) {

			/*if (!LocalPlayer::Entity() || LocalPlayer::Entity()->is_sleeping()) {
				UnloadChams();
			}*/

			if (VarSs::misc::silentstash && GetAsyncKeyState(VarSs::keys::stasher))
			{
				BaseEntity* stash_object = BaseNetworkable::clientEntities()->FindClosest<BaseEntity*>(STATIC_CRC32("StashContainer"), LocalPlayer::Entity(), 3.0f);
				if (stash_object && stash_object != nullptr)
					stash_object->_ServerRPC("RPC_OpenLoot");
			}

			if (VarSs::stuff::best_target != Vector3::Zero()) {
				target_player = nullptr;
			}

			if (VarSs::visuals::hand_chams_type == 0 || VarSs::players::player_chams_type == 0 || VarSs::visuals::weapon_chams_type == 0
				|| VarSs::visuals::hand_chams_type == 1 || VarSs::players::player_chams_type == 1 || VarSs::visuals::weapon_chams_type == 1
				|| VarSs::visuals::hand_chams_type == 2 || VarSs::players::player_chams_type == 2 || VarSs::visuals::weapon_chams_type == 2
				|| VarSs::visuals::hand_chams_type == 3 || VarSs::players::player_chams_type == 3 || VarSs::visuals::weapon_chams_type == 3
				|| VarSs::visuals::hand_chams_type == 4 || VarSs::players::player_chams_type == 4 || VarSs::visuals::weapon_chams_type == 4
				|| VarSs::visuals::hand_chams_type == 4 || VarSs::players::player_chams_type == 5 || VarSs::visuals::weapon_chams_type == 5
				|| VarSs::visuals::hand_chams_type == 5 || VarSs::players::player_chams_type == 6 || VarSs::visuals::weapon_chams_type == 6
				|| VarSs::visuals::hand_chams_type == 7 || VarSs::players::player_chams_type == 7 || VarSs::visuals::weapon_chams_type == 7
				|| VarSs::visuals::hand_chams_type == 8 || VarSs::players::player_chams_type == 8 || VarSs::visuals::weapon_chams_type == 8
				|| VarSs::visuals::hand_chams_type == 9 || VarSs::players::player_chams_type == 9 || VarSs::visuals::weapon_chams_type == 9
				|| VarSs::visuals::hand_chams_type == 10 || VarSs::players::player_chams_type == 10 || VarSs::visuals::weapon_chams_type == 10
				|| VarSs::visuals::hand_chams_type == 11 || VarSs::players::player_chams_type == 11 || VarSs::visuals::weapon_chams_type == 11
				|| VarSs::visuals::hand_chams_type == 12 || VarSs::players::player_chams_type == 12 || VarSs::visuals::weapon_chams_type == 12) {
				static auto Facz = AssetBundle::LoadFromFile("C:\\file");
				static auto Facz2 = AssetBundle::LoadFromFile("C:\\file2");

				if (VarSs::visuals::hand_chams_type == 0 || VarSs::visuals::weapon_chams_type == 0)
				{
					Gold = Facz->LoadAsset<Material>("faczad_galaxy4.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 1 || VarSs::visuals::weapon_chams_type == 1)
				{
					Fade = Facz->LoadAsset<Material>("faczad_grad.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 2 || VarSs::visuals::weapon_chams_type == 2)
				{
					Frame = Facz->LoadAsset<Material>("faczad_frame.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 3 || VarSs::visuals::weapon_chams_type == 3)
				{
					Rainbow = Facz->LoadAsset<Material>("faczad_ghost.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 4 || VarSs::visuals::weapon_chams_type == 4)
				{
					Ghost = Facz->LoadAsset<Material>("faczad_ghost2.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 5 || VarSs::visuals::weapon_chams_type == 5)
				{
					Glitch = Facz->LoadAsset<Material>("faczad_storm.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 6 || VarSs::visuals::weapon_chams_type == 6)
				{
					Glitch2 = Facz->LoadAsset<Material>("faczad_storm2.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 7 || VarSs::visuals::weapon_chams_type == 7)
				{
					Glitch3 = Facz->LoadAsset<Material>("faczad_galaxy.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 8 || VarSs::visuals::weapon_chams_type == 8)
				{
					Glitch4 = Facz->LoadAsset<Material>("faczad_galaxy2.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 9 || VarSs::visuals::weapon_chams_type == 9)
				{
					Galaxy = Facz->LoadAsset<Material>("faczad_galaxy3.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 10 || VarSs::visuals::weapon_chams_type == 10)
				{
					Galaxy2 = Facz->LoadAsset<Material>("faczad_galaxy4.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 11 || VarSs::visuals::weapon_chams_type == 11)
				{
					Galaxy3 = Facz->LoadAsset<Material>("faczad_galaxy5.mat", Type::Material());
				}
				if (VarSs::visuals::hand_chams_type == 12 || VarSs::visuals::weapon_chams_type == 12)
				{
					Galaxy4 = Facz->LoadAsset<Material>("faczad_galaxy6.mat", Type::Material());
				}
				if (VarSs::players::player_chams_type == 0)
				{
					Flat = Facz2->LoadAsset<Shader>("chams.shader", Type::Shader());
				}
				if (VarSs::players::player_chams_type == 1)
				{
					RainbowFrame = Facz2->LoadAsset<Shader>("wireframetransparent.shader", Type::Shader());
				}
				if (VarSs::players::player_chams_type == 2)
				{
					Glow = Facz2->LoadAsset<Shader>("chamslit.shader", Type::Shader());
				}
				if (VarSs::players::player_chams_type == 3 || VarSs::visuals::hand_chams_type == 11)
				{
					SetTrought = Facz2->LoadAsset<Shader>("seethroughshader.shader", Type::Shader());
				}
			}

			if (!plly) return original_clientinput(plly, ModelState);
			if (!plly->IsValid()) return original_clientinput(plly, ModelState);

			HandChams();


			if (utils::LineOfSight(reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head), LocalPlayer::Entity()->eyes()->position()))
				entities::TargetVisible = true;
			else
				entities::TargetVisible = false;

			if (VarSs::misc::local_bones)
			{
				last_penis_pos = target_player->transform_(penis)->position();
			}

			if (plly->userID() == LocalPlayer::Entity()->userID()) {

				auto held = plly->GetHeldEntity<BaseProjectile>();
				auto held_hammer = plly->GetHeldEntity<BaseMelee>();

				//if (held->class_name_hash() == STATIC_CRC32("Hammer")) {
				//	if (VarSs::weapons::hammer_spam && Math::Distance_3D(plly->get_bone_pos(head), LocalPlayer::Entity()->get_bone_pos(head)) <= 2.5f) {
				//		//lol::hammerspam_attack(active, true);
				//	}
				//}

				static float Alpha = 2500;
				static bool Tick = false;
				static float Speed = 8.0f;

				if (!Tick || Alpha != 2100)
				{
					Tick = false;
					if (!(Alpha >= 2500))
						Alpha += Speed;
					else if (Alpha >= 2500)
						Tick ^= 1;
					if (Alpha >= 2100)
					{
						Alpha = -300;
					}
				}

				const char* _text;
				static float r = 0.50f, g = 0.00f, b = 0.50f, a = 255.f;
				static int cases = 0;
				switch (cases)
				{
				case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
				case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
				case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
				case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
				default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
				}


				if (VarSs::players::playerline) {
					dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(VarSs::colors::PlayerLine.x, VarSs::colors::PlayerLine.y, VarSs::colors::PlayerLine.z, 1), 2.f, false, false);
				}

				if (VarSs::players::playerline3) {
					int col = rand() % 100;

					if (col > 90)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(254, 127, 127, 255), 2.5f, false, false);
					else if (col < 90 && col > 80)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(255, 128, 240, 255), 2.5f, false, false);
					else if (col < 80 && col > 70)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(174, 128, 255, 255), 2.5f, false, false);
					else if (col < 70 && col > 60)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(128, 148, 255, 255), 2.5f, false, false);
					else if (col < 60 && col > 50)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(128, 205, 255, 255), 2.5f, false, false);
					else if (col < 50 && col > 40)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(128, 255, 244, 255), 2.5f, false, false);
					else if (col < 40 && col > 30)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(128, 255, 152, 255), 2.5f, false, false);
					else if (col < 30 && col > 20)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(214, 255, 128, 255), 2.5f, false, false);
					else if (col < 20 && col > 10)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(255, 223, 128, 255), 2.5f, false, false);
					else if (col < 10)
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(255, 128, 128, 255), 2.5f, false, false);
					else
						dd1aw::Line(plly->_playerModel()->position() + Vector3(0, 0.25, 0), plly->lastSentTick()->position() + Vector3(0, 0.25, 0), Color(255, 255, 255, 255), 2.5f, false, false);
				}

				if (VarSs::misc::player_tp_head && GetAsyncKeyState(VarSs::keys::tp_head_key) && Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), target_player->get_bone_pos(head)) < 3.5f)
				{
					static float Alpha = 2500;
					static bool Tick = false;
					static float Speed = 0.5f;

					if (!Tick || Alpha != 2100)
					{
						Tick = false;
						if (!(Alpha >= 2500))
							Alpha += Speed;
						else if (Alpha >= 2500)
							Tick ^= 1;
						if (Alpha >= 2100)
						{
							Alpha = -300;
						}
					}

					const char* _text;
					static float r = 0.50f, g = 0.00f, b = 0.50f, a = 255.f;
					static int cases = 0;
					switch (cases)
					{
					case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
					case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
					case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
					case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
					default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
					}

					Vector3 curVel = LocalPlayer::Entity()->movement()->body()->velocity();
					Vector3 curVel2 = target_player->transform_(head)->position();
					Vector3 curVel3 = target_player->transform_(penis)->position();


					//dd1aw::Arrow(curVel3, LocalPlayer::Entity()->eyes()->position(), 8.5f, Color(r * 255, g * 255, b * 255, a), 0.01f);
					//dd1aw::Arrow(curVel2, LocalPlayer::Entity()->eyes()->position(), 8.0f, Color(r * 255, g * 255, b * 255, a), 0.01f)

					plly->movement()->TeleportTo(curVel2);
				}
				if (VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key))
					plly->clientTickInterval() = 0.75f;
				else {
					plly->clientTickInterval() = 0.05f;
				}

				/*if (VarSs::misc::rayleigh_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_rayleigh)(VarSs::misc::rayleigh);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_rayleigh)(VarSs::misc::rayleigh);
				}
				if (VarSs::misc::mie_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_mie)(VarSs::misc::mie);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_mie)(VarSs::misc::mie);
				}
				if (VarSs::misc::brightness_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_brightness)(VarSs::misc::brightness);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_atmosphere_brightness)(VarSs::misc::brightness);
				}
				if (VarSs::misc::thunder_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_thunder)(VarSs::misc::thunder);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_thunder)(VarSs::misc::thunder);
				}
				if (VarSs::misc::fog_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_fog)(VarSs::misc::fog);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_fog)(VarSs::misc::fog);
				}
				if (VarSs::misc::rain_changer) {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_rain)(VarSs::misc::rain);
				}
				else {
					reinterpret_cast<void(__fastcall*)(float)>(InGame::stor::gBase + CO::set_rain)(VarSs::misc::rain);
				}*/

				BaseMelee* melee = LocalPlayer::Entity()->GetHeldEntity<BaseMelee>();

				if (held) {
					BaseProjectile* _held = _held = plly->GetHeldEntity<BaseProjectile>();
					if (VarSs::weapons::no_sway && held->class_name_hash() != STATIC_CRC32("BaseMelee")) {
						held->aimSway() = 0.f;
						held->aimSwaySpeed() = 0.f;
					}
					//if  (_held->class_name_hash() == STATIC_CRC32("CrossBowWeapon")) {

					//}
					//else {
					//	if (VarSs::weapons::fastbow) {
					//		if (held->class_name_hash() != STATIC_CRC32("BowWeapon")) {
					//			held->attackReady() = true;
					//			held->arrowBack() = 1.f;
					//		}
					//	}
					//}

					if (VarSs::weapons::automatic)
						held->automatic() = true;
					//if (held->class_name_hash() == STATIC_CRC32("BaseMelee") || _held->class_name_hash() == STATIC_CRC32("BowWeapon") || _held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon") || _held->class_name_hash() == STATIC_CRC32("CrossBowWeapon")) {

					//}
					//else {
					if (VarSs::weapons::rapidfire) {
						0 == held->repeatDelay();
						held->repeatDelay() = VarSs::weapons::rapid_value;
					}
					//}
					/*if (held->class_name_hash() == STATIC_CRC32("BaseMelee") || _held->class_name_hash() == STATIC_CRC32("BowWeapon") || _held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon") || _held->class_name_hash() == STATIC_CRC32("CrossBowWeapon")) {

					}*/
					/*else {*/
					if (VarSs::weapons::roundfire) {
						0 == held->repeatDelay();
						held->repeatDelay() = 0.000000000000001f;
					}
					//}
					if (VarSs::weapons::weapon_spam) {
						if (GetAsyncKeyState(VarSs::keys::weaponspam)) {
							held->SendSignalBroadcast(BaseEntity::Signal::Attack, (""));
						}
					}

					if (VarSs::misc::Atom)
					{
						RaycastHit hitInfo;
						Quaternion q;
						bool valid = Physics::Raycast(LocalPlayer::Entity()->eyes()->position(), LocalPlayer::Entity()->eyes()->BodyForward(), &hitInfo, 400.f, 256 | 2048 | 65536 | 1073741824 | 8388608 | 2097152 | 4194304 | 67108864 | 134217728 | 33554432 | 32768 | 8192 | 512 | 1, QueryTriggerInteraction::Ignore);
						if (valid) {
							dd1aw::Capsule(hitInfo.point, q, 0.35f, 2 * 0.35f, Color(VarSs::colors::Boxviscol.x * 50, VarSs::colors::Boxviscol.y * 50, VarSs::colors::Boxviscol.z * 50, 50), 0.01f, false);
							if (VarSs::misc::AtomOutline)
							{
								dd1aw::Capsule(hitInfo.point, q, 0.40f, 2 * 0.40f, Color(0, 0, 0, 1), 0.01f, false);
							}

						}
					}

					if (VarSs::misc::circle_hat)
					{
						float step = (M_PI * 0.10f) / 72;

						for (float a = 0; a < (M_PI * 0.10f); a += step)
						{
							Vector3 actual_hat_pos = LocalPlayer::Entity()->get_bone_pos(head);

							Vector3 start(actual_hat_pos);
							Vector3 end(actual_hat_pos);

							dd1aw::Arrow(start, end,0.5f, Color(r * 255, g * 255, b * 255, a), 0.f);
						}
					}

					if (VarSs::combat::bullet_tp && target_player != nullptr) {
						auto transform = reinterpret_cast<BasePlayer*>(target_player)->transform_(head);
						if (transform) {
							transform->set_rotation(Quaternion(0.f, 0.f, 0.f, 1.f));
						}
					}

					float lastShotTime = _held->lastShotTime() - GLOBAL_TIME;
					float reloadTime = _held->nextReloadTime() - GLOBAL_TIME;
					float desyncTime = (Time::realtimeSinceStartup() - plly->lastSentTickTime()) - 0.03125 * 3;
					if (w_last_syringe == 0.f) {
						w_last_syringe = LocalPlayer::Entity()->lastSentTickTime();
					}
					Item* weapon = LocalPlayer::Entity()->GetHeldItem();
					if (VarSs::misc::faster_healing && (weapon->info()->itemid() == 1079279582 || weapon->info()->itemid() == -2072273936) && LocalPlayer::Entity()->health() < 99) {
						BaseProjectile* ent = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
						if (LocalPlayer::Entity()->lastSentTickTime() > w_last_syringe + 0.7f) {
							ent->_ServerRPC(("UseSelf"));
							w_last_syringe = LocalPlayer::Entity()->lastSentTickTime();
						}
					}
					if (held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
						if (VarSs::combat::autoreload)
						{
							BaseProjectile* ent = plly->GetHeldEntity<BaseProjectile>();

							if (!did_reload)
								time_since_last_shot = (Time::fixedTime() - fixed_time_last_shot);

							if (just_shot && (time_since_last_shot > 0.2f))
							{
								ent->_ServerRPC(("StartReload"));
								ent->SendSignalBroadcast(BaseEntity::Signal::Reload);
								just_shot = false;
							}
							if (time_since_last_shot > (_held->reloadTime() - (_held->reloadTime() / 10))
								&& !did_reload)
							{
								ent->_ServerRPC(("Reload"));
								did_reload = true;
								time_since_last_shot = 0;
							}
						}
					}

					Vector3 target;

					if (VarSs::combat::manipulator) {
						if (VarSs::stuff::best_target != Vector3(0, 0, 0))
						{
							target = VarSs::stuff::best_target;
						}
						else
						{
							target = reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head);
						}
					}

					if (VarSs::combat::autoshoot) {

						if (VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
							if (held && _held->class_name_hash() == STATIC_CRC32("BaseProjectile") || _held->class_name_hash() == STATIC_CRC32("BowWeapon") || _held->class_name_hash() == STATIC_CRC32("CrossBowWeapon"))
							{
								auto m = held->repeatDelay() * 0.75f;
								float timeSinceLastTick = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime());
								int r = timeSinceLastTick / m;
								if (VarSs::combat::instakill && r > 1) {
									PowerShoot(r, held, target, plly);
								}
								else {
									if (_held->HasAttackCooldown() || _held->nextAttackTime() >= Time::time() || _held->timeSinceDeploy() < _held->deployDelay()) {
									}
									else
									{
										if (!held->Empty()) {
											if (reloadTime < 1.0f && lastShotTime < -0.1f) {
												if (reinterpret_cast<BasePlayer*>(target_player) != nullptr && !LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(target_player)) && reinterpret_cast<BasePlayer*>(target_player)->IsValid() && plly->HasPlayerFlag(PlayerFlags::Connected)/* && !did_reload*/) {
													if (utils::LineOfSight(target, plly->eyes()->position())) {
														_held->SendSignalBroadcast(BaseEntity::Signal::Attack);
														_held->LaunchProjectile();
														_held->primaryMagazine()->contents()--;
														_held->UpdateAmmoDisplay();
														_held->ShotFired();
														_held->DidAttackClientside();
														_held->BeginCycle();
													}
												}
											}
										}
									}
								}
							}
						}
					}

					if (melee && melee != nullptr && melee->class_name_hash() == STATIC_CRC32("BaseMelee"))
					{
						auto melee_attack = [&](Vector3 pos, BaseEntity* p, BaseMelee* melee, bool is_player = false)
							{
								if (!p->IsValid() || !melee) return;
								if (melee->nextAttackTime() >= Time::fixedTime()) return;
								if (melee->deployDelay() > melee->timeSinceDeploy()) return;
								auto trans = is_player ? reinterpret_cast<BasePlayer*>(p)->transform_(head) : p->transform();
								if (!trans) return;
								Ray r = Ray(plly->eyes()->get_position(), (pos - LocalPlayer::Entity()->eyes()->get_position()).normalized());
								Vector3 origin = LocalPlayer::Entity()->eyes()->position();
								HitTest* hit = HitTest::New();
								hit->HitEntity() = p;
								hit->DidHit() = true;
								hit->MaxDistance() = melee->maxDistance();
								hit->HitTransform() = trans;
								hit->AttackRay() = r;
								hit->HitNormal() = trans->InverseTransformPoint(pos);
								hit->HitPoint() = trans->InverseTransformPoint(pos);
								melee->DoAttack(hit);
								melee->StartAttackCooldown(melee->repeatDelay());
							};
						if (VarSs::combat::silent_melee && reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head).distance(LocalPlayer::Entity()->eyes()->position()) <= melee->maxDistance() + 2.5f)
						{
							melee_attack(reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head), target_player, melee, true);

							//dd1aw::Arrow(melee_attack, LocalPlayer::Entity()->eyes()->position(), 8.5f, Color(r * 255, g * 255, b * 255, a), 0.01f);
						}

						//Vector3 LocalPos = LocalPlayer::Entity()->transform_(head)->position();
						//Vector3 LocalPosHead = LocalPlayer::Entity()->transform_(head)->position();
						//Vector3 LocalPosJaw = LocalPlayer::Entity()->transform_(jaw)->position();
						//Vector3 LocalPosNeck = LocalPlayer::Entity()->transform_(neck)->position();
						//Vector3 PlayerPos = reinterpret_cast<BasePlayer*>(target_player)->transform_(head)->position();
						//Vector3 MM_AITT = (LocalPosHead, LocalPosJaw, LocalPosNeck);

						//float Distance = Math::Distance_3D(MM_AITT, PlayerPos);

						//if (VarSs::combat::auto_manipulator) {

						//	if (Distance < VarSs::combat::auto_m_dist) {

						//		if (VarSs::combat::manipulator) {

						//			if (held && _held->class_name_hash() == STATIC_CRC32("BaseProjectile") || _held->class_name_hash() == STATIC_CRC32("BowWeapon") || _held->class_name_hash() == STATIC_CRC32("CrossBowWeapon"))
						//			{
						//				if (_held->HasAttackCooldown() || _held->nextAttackTime() >= Time::time() || _held->timeSinceDeploy() < _held->deployDelay()) {
						//				}
						//				else
						//				{
						//					if (!held->Empty()) {
						//						if (reloadTime < 1.0f && lastShotTime < -0.1f) {
						//							if (reinterpret_cast<BasePlayer*>(target_player) != nullptr && !LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(target_player)) && reinterpret_cast<BasePlayer*>(target_player)->IsValid() && plly->HasPlayerFlag(PlayerFlags::Connected)/* && !did_reload*/) {
						//								if (utils::LineOfSight(target, plly->eyes()->position())) {
						//									_held->SendSignalBroadcast(BaseEntity::Signal::Attack);
						//									_held->LaunchProjectile();
						//									_held->primaryMagazine()->contents()--;
						//									_held->UpdateAmmoDisplay();
						//									_held->ShotFired();
						//									_held->DidAttackClientside();
						//									_held->BeginCycle();
						//								}
						//							}
						//						}
						//					}
						//				}
						//			}
						//		}
						//	}
						//}

						//if (VarSs::combat::instakill && GetAsyncKeyState(VarSs::keys::instakill)) {
						//	if (!held->Empty()) {
						//		if (reinterpret_cast<BasePlayer*>(target_player) != nullptr && !LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(target_player)) && reinterpret_cast<BasePlayer*>(target_player)->IsValid() && plly->HasPlayerFlag(PlayerFlags::Connected)/* && !did_reload*/) {
						//			if (utils::LineOfSight(reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head), plly->eyes()->position())) {
						//				if (desyncTime > 0.80f) {
						//					plly->clientTickInterval() = 0.65f;
						//					_held->SendSignalBroadcast(BaseEntity::Signal::Attack);
						//					_held->LaunchProjectile();
						//					_held->primaryMagazine()->contents()--;
						//					_held->UpdateAmmoDisplay();
						//					_held->ShotFired();
						//					_held->DidAttackClientside();
						//					_held->BeginCycle();
						//				}
						//			}
						//		}
						//	}
						//}
					}

					if (melee && melee != nullptr && melee->class_name_hash() == STATIC_CRC32("BaseMelee"))
					{
						auto melee_attack = [&](Vector3 pos, BaseEntity* p, BaseMelee* melee, bool is_player = false)
							{
								if (!p->IsValid() || !melee) return;
								if (melee->nextAttackTime() >= Time::fixedTime()) return;
								if (melee->deployDelay() > melee->timeSinceDeploy()) return;
								auto trans = is_player ? reinterpret_cast<BasePlayer*>(p)->transform_(head) : p->transform();
								if (!trans) return;
								Ray r = Ray(plly->eyes()->get_position(), (pos - LocalPlayer::Entity()->eyes()->get_position()).normalized());
								Vector3 origin = LocalPlayer::Entity()->eyes()->position();
								HitTest* hit = HitTest::New();
								hit->HitEntity() = p;
								hit->DidHit() = true;
								hit->MaxDistance() = melee->maxDistance();
								hit->HitTransform() = trans;
								hit->AttackRay() = r;
								hit->HitNormal() = trans->InverseTransformPoint(pos);
								hit->HitPoint() = trans->InverseTransformPoint(pos);
								melee->DoAttack(hit);
								melee->StartAttackCooldown(melee->repeatDelay());
							};
						if (VarSs::combat::silent_melee && reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head).distance(LocalPlayer::Entity()->eyes()->position()) <= melee->maxDistance() + 2.5f)
						{
							melee_attack(reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head), target_player, melee, true);

							//dd1aw::Arrow(melee_attack, LocalPlayer::Entity()->eyes()->position(), 8.5f, Color(r * 255, g * 255, b * 255, a), 0.01f);
						}
						//hammer material spam
						if (VarSs::weapons::hammer_spam)
						{
							Vector3 LocalPos = LocalPlayer::Entity()->transform_(head)->position();
							Vector3 PlayerPos = reinterpret_cast<BasePlayer*>(target_player)->transform_(head)->position();

							float Distance = Math::Distance_3D(LocalPos, PlayerPos);

							if (Distance < 2.5f) {
								if (held_hammer->class_name_hash() == STATIC_CRC32("Hammer"))
								{
									if (!melee->HasAttackCooldown())
									{
										HitTest* hit = HitTest::New();

										auto playerPos = target_player->transform_(head)->position();

										hit->MaxDistance() = 1000.f;
										hit->HitTransform() = target_player->transform_(head);
										hit->AttackRay() = Ray(LocalPlayer::Entity()->eyes()->get_position(), (playerPos - LocalPlayer::Entity()->eyes()->get_position()).Normalized());
										hit->DidHit() = true;
										hit->HitEntity() = target_player;
										hit->HitPoint() = target_player->eyes()->transform()->InverseTransformPoint(target_player->eyes()->transform()->position() + Vector3(Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f)));
										hit->HitNormal() = target_player->eyes()->transform()->InverseTransformPoint(target_player->eyes()->transform()->position() + Vector3(Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f)));

										if (VarSs::weapons::hammer_spam_type == 0)
										{
											hit->HitMaterial_() = il2cpp::String::New(("glass"));
										}

										else if (VarSs::weapons::hammer_spam_type == 1)
										{
											hit->HitMaterial_() = il2cpp::String::New(("water"));
										}

										if (VarSs::weapons::hammer_spam_type == 2)
										{
											hit->HitMaterial_() = il2cpp::String::New(("forest"));
										}

										if (VarSs::weapons::hammer_spam_type == 3)
										{
											hit->HitMaterial_() = il2cpp::String::New(("metal"));
										}

										if (VarSs::weapons::hammer_spam_type == 4)
										{
											hit->HitMaterial_() = il2cpp::String::New(("explosions"));
										}

										melee->StartAttackCooldown(0.f);
										melee->ProcessAttack(hit);
									}
								}
							}
						}

						if (VarSs::misc::auto_farm) {
							BaseMelee* melee = LocalPlayer::Entity()->GetHeldEntity<BaseMelee>();
							auto jackhammerorchainsaw = LocalPlayer::Entity()->GetHeldEntity();
							if (melee && melee != nullptr && melee->class_name_hash() == STATIC_CRC32("BaseMelee") ||
								jackhammerorchainsaw && jackhammerorchainsaw != nullptr && melee->class_name_hash() == STATIC_CRC32("Jackhammer"))
							{
								if (VarSs::misc::auto_farm_ore)
								{
									BaseNetworkable* marker = BaseNetworkable::clientEntities()->FindClosest(STATIC_CRC32("OreHotSpot"), LocalPlayer::Entity(), melee->maxDistance() + 1.8f);
									if (marker && marker != nullptr && marker->transform()->position().distance(LocalPlayer::Entity()->eyes()->position()) <= melee->maxDistance() + 1.8f)
									{
										if (!(melee->nextAttackTime() >= Time::fixedTime())) {
											if (!(melee->deployDelay() > melee->timeSinceDeploy())) {
												Vector3 origin = LocalPlayer::Entity()->eyes()->position();
												Vector3 playerPos = marker->transform()->position();
												HitTest* hit = HitTest::New();
												hit->_HitEntity() = marker;
												hit->DidHit() = true;
												hit->MaxDistance() = melee->maxDistance();
												hit->HitTransform() = marker->transform();
												hit->AttackRay() = Ray(origin, origin + (playerPos - origin));
												hit->HitNormal() = marker->transform()->InverseTransformPoint(marker->transform()->position());
												hit->HitPoint() = marker->transform()->InverseTransformPoint(marker->transform()->position());
												melee->DoAttack(hit);
												melee->StartAttackCooldown(melee->repeatDelay());
											}
										}

									}
								}
								if (VarSs::misc::auto_farm_tree)
								{
									BaseNetworkable* TreeEntity = BaseNetworkable::clientEntities()->FindClosest(STATIC_CRC32("TreeEntity"), LocalPlayer::Entity(), melee->maxDistance() + 1.8f);
									if (TreeEntity && TreeEntity->transform()->position().distance(LocalPlayer::Entity()->eyes()->position()) <= melee->maxDistance() + 1.8f)
									{
										BaseNetworkable* tree = BaseNetworkable::clientEntities()->FindClosest(STATIC_CRC32("TreeMarker"), TreeEntity, melee->maxDistance() + 1.8f);
										if (tree && tree->transform()->position().distance(LocalPlayer::Entity()->eyes()->position()) <= melee->maxDistance() + 1.8f)
										{
											if (!(melee->nextAttackTime() >= Time::fixedTime())) {
												if (!(melee->deployDelay() > melee->timeSinceDeploy())) {
													Vector3 origin = LocalPlayer::Entity()->eyes()->position();
													Vector3 treepos = TreeEntity->transform()->position();
													treepos.y = origin.y;
													HitTest* hit = HitTest::New();
													hit->_HitEntity() = TreeEntity;
													hit->DidHit() = true;
													hit->HitMaterial_() = il2cpp::String::New(("Wood"));
													hit->MaxDistance() = melee->maxDistance();
													hit->HitTransform() = TreeEntity->transform();
													hit->AttackRay() = Ray(origin, treepos - origin);
													hit->HitNormal() = TreeEntity->transform()->InverseTransformPoint(treepos);
													hit->HitPoint() = TreeEntity->transform()->InverseTransformPoint(treepos);

													melee->DoAttack(hit);
													melee->StartAttackCooldown(melee->repeatDelay());
												}
											}

										}
									}

								}
							}
						}
					}
					if (VarSs::misc::local_bones)
					{
						//dd1aw::Line(localPlayer->eyes( )->get_position( ), ret->hitPositionWorld( ), Color(1, 0, 0, 1), 0.05f, false, true);
						dd1aw::Sphere(last_penis_pos, 0.01f, Color(r * 255, g * 255, b * 255, a), 0.01f, false); //penis 1
						dd1aw::Sphere(last_penis_pos, 0.03f, Color(r * 255, g * 255, b * 255, a), 0.03f, false); //penis 2	
						dd1aw::Sphere(last_penis_pos, 0.05f, Color(r * 255, g * 255, b * 255, a), 0.05f, false); //penis 3	
					}
					if (VarSs::visuals::No_bobing)
					{
						if (auto ActiveModel = BaseViewModel::ActiveModel())
						{
							auto bob = ActiveModel->bob();

							auto lower = ActiveModel->lower();

							auto sway = ActiveModel->sway();

							bob->bobAmountRun() = 0.f;
							bob->bobAmountWalk() = 0.f;
							bob->bobSpeedRun() = 0.f;
							bob->bobSpeedWalk() = 0.f;
							lower->shouldLower() = false;
						}
					}

					if (VarSs::weapons::compound && held->class_name_hash() == STATIC_CRC32("CompoundBowWeapon")) {
						reinterpret_cast<CompoundBowWeapon*>(held)->currentHoldProgress() = 1.5f;
					}

					if (VarSs::weapons::remove_attack_anim && held && _held->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
						if (auto ActiveModel = BaseViewModel::ActiveModel())
						{
							ActiveModel->animator() = 0;
						}
					}

					/*if (VarSs::weapons::fastbow && held->class_name_hash() == STATIC_CRC32("BowWeapon")) {
						held->attackReady() = true;
						held->arrowBack() = 1.f;
					}*/

				}
				else {
					did_reload = false;
					just_shot = true;
					fixed_time_last_shot = Time::fixedTime();
				}
				if (VarSs::misc::auto_pickup)
				{
					BaseEntity* collectibleEntity = BaseNetworkable::clientEntities()->FindClosest<BaseEntity*>(STATIC_CRC32("CollectibleEntity"), LocalPlayer::Entity(), 3.0f);
					if (collectibleEntity && collectibleEntity != nullptr && collectibleEntity != 0x0)
						collectibleEntity->_ServerRPC("Pickup");
				}

				float changeSpeed = 0.2f;
				float FOV = VarSs::misc::fov;

				if (GetAsyncKeyState(VarSs::keys::zoom))
				{
					float oldFOV = ConVar::Graphics::_fov();
					float newFOV = oldFOV + (15.f - oldFOV) * changeSpeed;
					ConVar::Graphics::_fov() = newFOV;
				}
				else
				{
					float zoomfov = ConVar::Graphics::_fov();
					float newFOV = zoomfov + (FOV - zoomfov) * changeSpeed;
					ConVar::Graphics::_fov() = newFOV;
				}
				if (VarSs::misc::custom_time)
					BasePlayer::Admintime() = VarSs::misc::time;
				if (VarSs::misc::night_mode) {
					BasePlayer::Admintime() = 0.f;
				}
				if (VarSs::misc::AspectRatio)
					Camera::SetAspect(VarSs::misc::ratio);
				if (VarSs::misc::fakeadmin)
					plly->playerFlags() |= PlayerFlags::IsAdmin;
				float nextActionTime = 0, period = 1.4721, last_gesture_rpc = 0.f;;
				if (VarSs::misc::gesture > 0
					&& Time::fixedTime() > last_gesture_rpc + 0.35f)
				{
					switch (VarSs::misc::gesture) {
					case 0:
						break;
					case 1:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("clap"));
						break;
					case 2:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("friendly"));
						break;
					case 3:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("thumbsdown"));
						break;
					case 4:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("thumbsup"));
						break;
					case 5:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("ok"));
						break;
					case 6:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("point"));
						break;
					case 7:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("shrug"));
						break;
					case 8:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("victory"));
						break;
					case 9:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("wave"));
						break;
					case 10:
						LocalPlayer::Entity()->SendSignalBroadcast(BaseEntity::Signal::Gesture, ("dance"));
						break;
					}
					last_gesture_rpc = Time::fixedTime();
				}
				if (VarSs::misc::suicide && GetAsyncKeyState(VarSs::keys::suicide)) {
					LocalPlayer::Entity()->OnLand(-108.0001f);
				}
				GLOBAL_TIME = Time::time();
				if (target_player != nullptr && VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
					other::find_manipulate_angle();
				}
				else if (!other::m_manipulate.empty()) {
					other::m_manipulate = Vector3::Zero();
				}
				if (VarSs::weapons::minicopter_aim)
					if (plly->mounted())
						plly->mounted()->canWieldItems() = true;

				Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::Water, !VarSs::misc::jesus);
				Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::Tree, VarSs::misc::jesus);
				Physics::IgnoreLayerCollision((int)Layer::PlayerMovement, (int)Layer::AI, VarSs::misc::jesus);
				if (VarSs::misc::spiderman) {
					LocalPlayer::Entity()->movement()->groundAngle() = 0.f;
					LocalPlayer::Entity()->movement()->groundAngleNew() = 0.f;
				}
				if (VarSs::misc::gravity) {
					LocalPlayer::Entity()->gravityModifier() = VarSs::misc::gravity_modifier;
					//LocalPlayer::Entity()->gravityModifier()->SetGravity = (VarSs::misc::gravity_modifier);
				}
				if (VarSs::visuals::skycolor)
				{
					UINT64 klass = read(InGame::stor::gBase + 61244128, UINT64);// TOD_Sky_c
					UINT64 static_fields = read(klass + 0xB8, UINT64);
					UINT64 instances = read(static_fields, UINT64);
					UINT64 List1 = read(instances + 0x10, UINT64);
					UINT64 TOD_Sky_ = read(List1 + 0x20, UINT64);

					auto days = *reinterpret_cast<DWORD64*>(TOD_Sky_ + 0x50);
					auto dsky_color = *reinterpret_cast<DWORD64*>(days + 0x28);
					auto clr = Color(VarSs::colors::sky_color.x, VarSs::colors::sky_color.y, VarSs::colors::sky_color.z, 1);
					auto dsky_gradient = *reinterpret_cast<DWORD64*>(dsky_color + 0x10);
					*reinterpret_cast<Color*>(dsky_gradient) = clr;

					auto nights = *reinterpret_cast<DWORD64*>(TOD_Sky_ + 0x58);
					auto nsky_color = *reinterpret_cast<DWORD64*>(nights + 0x28);
					auto nsky_gradient = *reinterpret_cast<DWORD64*>(nsky_color + 0x10);
					*reinterpret_cast<Color*>(nsky_gradient) = clr;

				}
			}
			float flyhack_forgiveness_interia = 10.0f;
			float flyhack_forgiveness = 1.0f;
			float flyhack_extrusion = 2.0f;
			bool flag = false;
			bool isInAir = false;
			Vector3 vector = (plly->lastSentTick()->position() + plly->transform()->position()) * 0.5f;
			flyhackPauseTime = Mathf::Max(0.f, flyhackPauseTime - Time::deltaTime());

			if (!plly->OnLadder() && !WaterLevel::Test(vector - Vector3(0.0f, flyhack_extrusion, 0.0f), false, plly)) {
				float radius = plly->GetRadius();
				float height = plly->GetHeight(false);

				Vector3 vector2 = vector + Vector3(0.0f, radius - flyhack_extrusion, 0.0f);
				Vector3 vector3 = vector + Vector3(0.0f, height - radius, 0.0f);
				float radius2 = radius - 0.05f;

				isInAir = !GamePhysics::CheckCapsule(vector2, vector3, radius2, 1503731969, GamePhysics::QueryTriggerInteraction::Ignore);

				if (isInAir)
				{
					Vector3 vector4 = (plly->transform()->position() - plly->lastSentTick()->position());
					float num2 = Mathf::Abs(vector4.y);
					float num3 = Misc::Magnitude2D(vector4);
					if (vector4.y >= 0.0f)
					{
						flyhackDistanceVertical += vector4.y;
						flag = true;
					}
					if (num2 < num3)
					{
						flyhackDistanceHorizontal += num3;
						flag = true;
					}
					if (flag)
					{
						float num5 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
						float num6 = (plly->GetJumpHeight() + num5);

						float num7 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
						float num8 = (5.f + num7);

					}

				}
				else {
					flyhackDistanceHorizontal = 0.f;
					flyhackDistanceVertical = 0.f;
				}
				float num5 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
				float num6 = ((plly->GetJumpHeight() + num5) * 3);
				VMaxFlyhack = num6;
				if (flyhackDistanceVertical <= (num6)) {
					VFlyhack = flyhackDistanceVertical;
				}
				if (VFlyhack >= VMaxFlyhack)
					VFlyhack = VMaxFlyhack;
				float num7 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
				float num8 = ((4.f + num7) * 3);
				HMaxFlyhack = num8;
				if (flyhackDistanceHorizontal <= (num8)) {
					HFlyhack = flyhackDistanceHorizontal;
				}
				if (HFlyhack >= HMaxFlyhack)
					HFlyhack = HMaxFlyhack;

			}
			else {
				flyhackDistanceHorizontal = 0.f;
				flyhackDistanceVertical = 0.f;
			}
			if (!isInAir)
			{
				flyhackDistanceHorizontal = 0.f;
				flyhackDistanceVertical = 0.f;
			}
			if (WaterLevel::Test(vector - Vector3(0.0f, flyhack_extrusion, 0.0f), false, plly))
			{
				flyhackDistanceHorizontal = 0.f;
				flyhackDistanceVertical = 0.f;
			}
			original_clientinput(plly, ModelState);

			switch (VarSs::misc::modelstate)
			{
			case 1:
				LocalPlayer::Entity()->modelState()->flags() |= (int)ModelStateFlag::OnLadder;

				break;
			case 2:
				LocalPlayer::Entity()->modelState()->flags() |= (int)ModelStateFlag::OnGround;
				break;
			case 3:
				LocalPlayer::Entity()->modelState()->flags() |= (int)ModelStateFlag::Ducked;
				break;
			case 4:
				LocalPlayer::Entity()->modelState()->flags() |= (int)ModelStateFlag::Sleeping;
				break;

			}
			if (VarSs::misc::longneck) {
				if (GetAsyncKeyState(VarSs::keys::longnecking)) {
					float desyncTime = (Time::realtimeSinceStartup() - plly->lastSentTickTime()) - 0.03125 * 3;
					float max_eye_value = (0.1f + ((desyncTime + 2.f / 40.f + 0.125f) * 1.5f) * plly->MaxVelocity()) - 0.05f;
					plly->eyes()->viewOffset() = Vector3(0, max_eye_value, 0);
				}
			}

			if (VarSs::misc::omnidirectional_sprinting) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Sprinting);
			}
			if (VarSs::misc::speedhack) {
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Sprinting);
			}
			if (VarSs::misc::interactive_debug && GetAsyncKeyState(VarSs::keys::debugging)) {
				BaseProjectile* ent = plly->GetHeldEntity<BaseProjectile>();
				LocalPlayer::Entity()->add_modelstate_flag(ModelStateFlag::Mounted);
				ent->_ServerRPC(("LootPlayer"));
			}
		}

		void UpdateAmbient(TOD_Sky* TOD_Sky) {
			if (VarSs::misc::night_stars) {
				uintptr_t stars = read(TOD_Sky + 0x70, uintptr_t);
				*(float*)(stars + 0x14) = VarSs::misc::stars;
			}
			if (!VarSs::misc::bright_ambient) {
				return original_updateambient(TOD_Sky);
			}
			RenderSettings::set_ambientMode(RenderSettings::AmbientMode::Flat);
			RenderSettings::set_ambientIntensity(6.f);
			RenderSettings::set_ambientLight(Color({ VarSs::colors::ambient_color.x, VarSs::colors::ambient_color.y, VarSs::colors::ambient_color.z, 1 }));
			if (VarSs::misc::night_mode) {
				RenderSettings::set_ambientLight(Color({ 0, 0, 1, 1 }));
			}
		}
		pUncStr Run(ConsoleOptions* options, pUncStr strCommand, DWORD64 args) {
			if (options->IsFromServer()) {
				std::wstring cmd = std::wstring(strCommand->str);
				if (cmd.find((L"noclip")) != std::wstring::npos || cmd.find((L"debugcamera")) != std::wstring::npos || cmd.find((L"camlerp")) != std::wstring::npos || cmd.find((L"camspeed")) != std::wstring::npos || cmd.find((L"admintime")) != std::wstring::npos) {
					strCommand = nullptr;
				}
			}
			return original_consolerun(options, strCommand, args);
		}
	}
	namespace combat {
		inline void DoAttack_hk(FlintStrikeWeapon* weapon) noexcept {
			if (!weapon) return;
			if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return weapon->DoAttack();
			if (VarSs::weapons::eokatap) weapon->_didSparkThisFrame() = true;
			return weapon->DoAttack();
		}
		float GetRandomVelocity(ItemModProjectile* self) {
			if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return self->GetRandomVelocity();
			float modifier = 1.0f;
			if (VarSs::weapons::fast_bullets) modifier += 0.49f;
			return self->GetRandomVelocity() * modifier;
		}
		void AddPunch(HeldEntity* a1, Vector3 a2, float duration) {
			if (VarSs::weapons::no_recoil) {
				a2 *= VarSs::weapons::recoil_control / 100.f;
			}
			return original_addpunch(a1, a2, duration);
		}
		inline Vector3 MoveTowards_hk(Vector3 current, Vector3 target, float maxDelta) {
			if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return Vector3_::MoveTowards(current, target, maxDelta);
			static auto ptr = METHOD("Assembly-CSharp::BaseProjectile::SimulateAimcone(): Void");
			if (!ptr) return Vector3_::MoveTowards(current, target, maxDelta);
			if (CALLED_BY(ptr, 0x800)) {
				target *= VarSs::weapons::recoil_control / 100.f;
				maxDelta *= VarSs::weapons::recoil_control / 100.f;
			}
			return Vector3_::MoveTowards(current, target, maxDelta);
		}
		bool DoHit(Projectile* prj, HitTest* test, Vector3 point, Vector3 normal) {
			auto localPlayer = LocalPlayer::Entity();
			auto held = localPlayer->GetHeldEntity<BaseProjectile>();
			if (prj->isAuthoritative()) {

				if (VarSs::combat::ignore_team) {
					if (test->HitEntity() != null) {
						if (test->HitEntity()->IsValid()) {
							if (LocalPlayer::Entity()->is_teammate((uintptr_t)reinterpret_cast<BasePlayer*>(test->HitEntity()))) {
								if (reinterpret_cast<BaseCombatEntity*>(test->HitEntity())->IsPlayer()) {
									return false;
								}
							}
						}
					}
				}

				if (VarSs::misc::HitEffect) {
					if (VarSs::misc::hitmaterial == 0) {
					}
					if (VarSs::misc::hitmaterial == 1) {
						test->HitMaterial_() = il2cpp::String::New(("glass"));
					}
					if (VarSs::misc::hitmaterial == 2) {
						test->HitMaterial_() = il2cpp::String::New(("water"));
					}
					if (VarSs::misc::hitmaterial == 3) {
						test->HitMaterial_() = il2cpp::String::New(("wood"));
					}
					if (VarSs::misc::hitmaterial == 4) {
						test->HitMaterial_() = il2cpp::String::New(("metal"));
					}
					if (VarSs::misc::hitmaterial == 5) {
						test->HitMaterial_() = il2cpp::String::New(("sand"));
					}
					if (VarSs::misc::hitmaterial == 6) {
						test->HitMaterial_() = il2cpp::String::New(("grass"));
					}
					if (VarSs::misc::hitmaterial == 7) {
						test->HitMaterial_() = il2cpp::String::New(("rock"));
					}
					if (VarSs::misc::hitmaterial == 8) {
						test->HitMaterial_() = il2cpp::String::New(("concrete"));
					}
					if (VarSs::misc::hitmaterial == 9) {
						test->HitMaterial_() = il2cpp::String::New(("forest"));
					}
					if (VarSs::misc::hitmaterial == 10) {
						test->HitMaterial_() = il2cpp::String::New(("cloth"));
					}
					if (VarSs::misc::hitmaterial == 11) {
						test->HitMaterial_() = il2cpp::String::New(("explosions"));
					}
				}
				if (VarSs::misc::HitSpawnNearest) {
					int num = rand() % 100;

					if (num > 90)
						test->HitMaterial_() = il2cpp::String::New(("glass"));
					else if (num < 90 && num > 80)
						test->HitMaterial_() = il2cpp::String::New(("wood"));
					else if (num < 80 && num > 70)
						test->HitMaterial_() = il2cpp::String::New(("metal"));
					else if (num < 70 && num > 60)
						test->HitMaterial_() = il2cpp::String::New(("sand"));
					else if (num < 60 && num > 50)
						test->HitMaterial_() = il2cpp::String::New(("grass"));
					else if (num < 50 && num > 40)
						test->HitMaterial_() = il2cpp::String::New(("rock"));
					else if (num < 40 && num > 30)
						test->HitMaterial_() = il2cpp::String::New(("concrete"));
					else if (num < 30 && num > 20)
						test->HitMaterial_() = il2cpp::String::New(("forest"));
					else if (num < 20 && num > 10)
						test->HitMaterial_() = il2cpp::String::New(("cloth"));
					else if (num < 10)
						test->HitMaterial_() = il2cpp::String::New(("explosions"));
					else
						test->HitMaterial_() = il2cpp::String::New(("glass"));
				}
				if (VarSs::weapons::pierce_materials) {
					if (test->HitEntity() != null) {
						if (test->HitEntity()->IsValid()) {
							BaseCombatEntity* lol = reinterpret_cast<BaseCombatEntity*>(test->HitEntity());

							if (lol->ClassNameHash() == STATIC_CRC32("CargoShip") || lol->ClassNameHash() == STATIC_CRC32("BaseOven")
								|| lol->ClassNameHash() == STATIC_CRC32("TreeEntity") || lol->ClassNameHash() == STATIC_CRC32("OreResourceEntity")
								|| lol->ClassNameHash() == STATIC_CRC32("CH47HelicopterAIController") || lol->ClassNameHash() == STATIC_CRC32("MiniCopter")
								|| lol->ClassNameHash() == STATIC_CRC32("BoxStorage") || lol->ClassNameHash() == STATIC_CRC32("Workbench")
								|| lol->ClassNameHash() == STATIC_CRC32("VendingMachine") || lol->ClassNameHash() == STATIC_CRC32("Barricade")
								|| lol->ClassNameHash() == STATIC_CRC32("BuildingPrivlidge") || lol->ClassNameHash() == STATIC_CRC32("LootContainer")
								|| lol->ClassNameHash() == STATIC_CRC32("HackableLockedCrate") || lol->ClassNameHash() == STATIC_CRC32("ResourceEntity")
								|| lol->ClassNameHash() == STATIC_CRC32("RidableHorse") || lol->ClassNameHash() == STATIC_CRC32("MotorRowboat")
								|| lol->ClassNameHash() == STATIC_CRC32("ScrapTransportHelicopter") || lol->ClassNameHash() == STATIC_CRC32("JunkPile")
								|| lol->ClassNameHash() == STATIC_CRC32("MiningQuarry") || lol->ClassNameHash() == STATIC_CRC32("WaterCatcher")) {
								return false;
							}
						}
					}
				}
			}

			return original_dohit(prj, test, point, normal);
		}

		bool CanAttack(BasePlayer* a1) {
			if (VarSs::misc::can_attack)
				return true;
			return original_canattack(a1);
		}
		Projectile* CreateProjectile(BaseProjectile* BaseProjectileA, void* prefab_pathptr, Vector3 pos, Vector3 forward, Vector3 velocity) {
			Projectile* projectile = original_create_projectile(BaseProjectileA, prefab_pathptr, pos, forward, velocity);
			BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(target_player);
			if (VarSs::weapons::size_bullet) {
				projectile->transform()->set_scale({ VarSs::weapons::bullet_sizex, VarSs::weapons::bullet_sizey, VarSs::weapons::bullet_sizez });
			}

			return projectile;
		}
		Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
			Vector3 Local = LocalPlayer::Entity()->eyes()->get_position();
			Vector3 heli_target = target_player->transform()->position() + Vector3(0, 1.5, 0);

			Vector3 target;

			if (target_player) {
				if (VarSs::combat::HitScan) {
					if (VarSs::stuff::best_target != Vector3(0, 0, 0))
					{
						target = VarSs::stuff::best_target;
					}
					else
					{
						target = target_player->get_bone_pos(head);
					}
				}
			}

			auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			BaseProjectile* _held = held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();

			if (held->class_name_hash() == STATIC_CRC32("BaseMelee")) {

			}
			else {
				if (VarSs::combat::psilent) {
					if (target_player) {
						auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
						BaseProjectile* _held = held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
						auto mag = held->primaryMagazine();
						if (!mag) return original_aimconedirection(aimCone, inputVec, anywhereInside);
						auto ammo = mag->ammoType();
						if (!ammo) return original_aimconedirection(aimCone, inputVec, anywhereInside);
						auto mod = ammo->GetComponent<ItemModProjectile>(Type::ItemModProjectile());
						if (!mod) return original_aimconedirection(aimCone, inputVec, anywhereInside);
						auto projectile = mod->projectileObject()->Get()->GetComponent<Projectile>(Type::Projectile());
						if (!projectile) return original_aimconedirection(aimCone, inputVec, anywhereInside);

						Vector3 LocalPos = LocalPlayer::Entity()->transform_(head)->position();
						Vector3 PlayerPos = reinterpret_cast<BasePlayer*>(target_player)->transform_(head)->position();

						float Distance = Math::Distance_3D(LocalPos, PlayerPos);

						if (projectile)
						{
							auto projectileVelocity = mod->projectileVelocity();

							auto PVS = _held->projectileVelocityScale();

							if (VarSs::weapons::fast_bullets) {
								PVS = (1.500f) * _held->projectileVelocityScale();
							}
							else
							{
								PVS = (1.000f) * _held->projectileVelocityScale();
							}

							float drag = projectile->drag();
							float gravityModifier = projectile->gravityModifier();
							float initialDistance = projectile->initialDistance();
							auto gravity = Physics::get_gravity();
							auto deltaTime = Time::fixedDeltaTime();
							auto timescale = Time::timeScale();

							float DeltaForLoop = deltaTime * timescale;

							Vector3 localPos = LocalPlayer::Entity()->eyes()->get_position();

							Vector3 actualTargetPos = target_player->get_bone_pos(head);

							if (VarSs::combat::HitScan) {
								if (VarSs::stuff::best_target != Vector3(0, 0, 0))
								{
									actualTargetPos = VarSs::stuff::best_target;
								}
								else
								{
									actualTargetPos = target_player->get_bone_pos(head);
								}
							}

							float bulletTime = std::sqrt((actualTargetPos.x) * (actualTargetPos.x) + (actualTargetPos.z) * (actualTargetPos.z));

							Vector3 targetvel = target_player->GetWorldVelocity();
							Vector3 targetPosition = actualTargetPos;

							auto _aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);

							auto position = localPos;
							float num = 0.03125f;
							int num3 = (8.f / DeltaForLoop);

							int simulations = 0;

							while (simulations < 30)
							{
								simulations++;
								bool hitPlayer = false;

								_aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
								Vector3 velocity = _aimDir * projectileVelocity * PVS;

								auto currentPosition = localPos;
								auto previousPosition = currentPosition;


								Vector3 closestPoint(FLT_MAX, FLT_MAX, FLT_MAX);
								Vector3 offset = Vector3().Zero();

								for (int i = -1; i < num3; i++)
								{
									previousPosition = currentPosition;
									currentPosition += velocity * deltaTime;
									velocity += gravity * gravityModifier * deltaTime;
									velocity -= velocity * drag * deltaTime;

									auto line = (currentPosition - previousPosition);
									auto len = line.UnityMagnitude();
									line.UnityNormalize();
									auto v = actualTargetPos - previousPosition;
									auto d = Vector3().UnityDot(v, line);

									if (d < 0.f)
									{
										d = 0.f;
									}
									else if (d > len)
									{
										d = len;
									}

									Vector3 nearestPoint = previousPosition + line * d;

									if (nearestPoint.distance(actualTargetPos) < 0.1f)
									{
										bulletTime = i * num;
										hitPlayer = true;
									}
									else if (nearestPoint.distance(actualTargetPos) < closestPoint.distance(actualTargetPos))
									{
										closestPoint = nearestPoint;
										offset = actualTargetPos - nearestPoint;
									}
								}
								if (hitPlayer) break;
								targetPosition += offset;
							}


							if (bulletTime != 1337.f)
							{
								Vector3 finalVelocity = Vector3(target_player->GetWorldVelocity().x, 0, target_player->GetWorldVelocity().z) * 0.75f * bulletTime;

								actualTargetPos += finalVelocity;

								Vector3 targetPosition = actualTargetPos;


								auto _aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
								float bulletTime = 1337.f;
								int sims = 0;
								while (sims < 30)
								{

									sims++;
									bool hitPlayer = false;

									_aimDir = original_aimconedirection(0.f, targetPosition - localPos, false);
									Vector3 velocity = _aimDir * projectileVelocity * PVS;

									auto currentPosition = localPos;
									auto previousPosition = currentPosition;

									Vector3 closestPoint(FLT_MAX, FLT_MAX, FLT_MAX);
									Vector3 offset = Vector3().Zero();

									for (int i = -1; i < num3; i++)
									{
										previousPosition = currentPosition;
										currentPosition += velocity * num;
										velocity += gravity * gravityModifier * num;
										velocity -= velocity * drag * num;

										auto line = (currentPosition - previousPosition);
										auto len = line.UnityMagnitude();
										line.UnityNormalize();
										auto v = actualTargetPos - previousPosition;
										auto d = Vector3().UnityDot(v, line);

										if (d < 0.f)
										{
											d = 0.f;
										}
										else if (d > len)
										{
											d = len;
										}

										Vector3 nearestPoint = previousPosition + line * d;

										if (nearestPoint.distance(actualTargetPos) < 0.1f)
										{
											bulletTime = i * num;
											hitPlayer = true;
										}
										else if (nearestPoint.distance(actualTargetPos) < closestPoint.distance(actualTargetPos))
										{
											closestPoint = nearestPoint;
											offset = actualTargetPos - nearestPoint;
										}
									}

									if (hitPlayer) break;
									targetPosition += offset;
								}

								did_reload = false;
								just_shot = true;
								fixed_time_last_shot = Time::fixedTime();

								return _aimDir;
							}
						}
					}
					else {

					}
				}
			}

			if (VarSs::weapons::no_spread) {
				aimCone = 0.f;
			}
			did_reload = false;
			just_shot = true;
			fixed_time_last_shot = Time::fixedTime();
			//auto held = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
			return original_aimconedirection(aimCone, inputVec, anywhereInside);
		}
		Vector3 hk_BodyLeanOffset(PlayerEyes* a1) {
			if (VarSs::combat::manipulator && !other::m_manipulate.empty()) {
				return other::m_manipulate;
			}
			return Original_BodyLeanOffset(a1);
		}
		Vector3 hk_EyePositionForPlayer(BaseMountable* arg1, BasePlayer* arg2, Quaternion* arg3) {
			BasePlayer* player = arg2;
			if (player->userID()) {
				if (VarSs::combat::manipulator && GetAsyncKeyState(VarSs::keys::manipulated_key)) {
					return Original_EyePositionForPlayer(arg1, arg2, arg3) + other::m_manipulate;
				}
			}
			return Original_EyePositionForPlayer(arg1, arg2, arg3);
		}
		inline Attack* BuildAttackMessage_hk(HitTest* self, BaseEntity* a2) noexcept {
			if (!self) return self->BuildAttackMessage(a2);
			auto ret = self->BuildAttackMessage(a2);
			auto entity = BaseNetworkable::clientEntities()->Find<BasePlayer*>(ret->hitID());

			if (!entity->IsPlayer() || VarSs::combat::always_heli_rotor)
			{
				if (entity->class_name_hash() == STATIC_CRC32("BaseHelicopter"))
				{
					if (entity->health() <= 5000.0f)
						ret->hitBone() = utils::StringPool::Get(("tail_rotor_col"));
					else
						ret->hitBone() = utils::StringPool::Get(("engine_col"));
				}
			}

			auto localPlayer = LocalPlayer::Entity();
			if (localPlayer) {

				if (reinterpret_cast<BasePlayer*>(self->ignoreEntity())->userID() == localPlayer->userID()) {
					if (entity) {
						if (entity->IsPlayer()) {
							if (VarSs::weapons::thick_bullet) {
								auto bone = entity->model()->find_bone(ret->hitPositionWorld());
								if (bone.second) {
									ret->hitPositionWorld() = bone.first->position();
								}
							}
							if (VarSs::combat::hitbox != 0) {
								if (VarSs::combat::hitbox == 1)
									ret->hitBone() = utils::StringPool::Get(("spine4"));
								else if (VarSs::combat::hitbox == 2)
									ret->hitBone() = utils::StringPool::Get(("head"));
								else if (VarSs::combat::hitbox == 3) {
									int yeet = rand() % 100;
									if (yeet > 50)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else
										ret->hitBone() = utils::StringPool::Get(("spine4"));
								}
								else if (VarSs::combat::hitbox == 4) {
									int num = rand() % 100;

									if (num > 90)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 90 && num > 80)
										ret->hitBone() = utils::StringPool::Get(("neck"));
									else if (num < 80 && num > 70)
										ret->hitBone() = utils::StringPool::Get(("l_clavicle"));
									else if (num < 70 && num > 60)
										ret->hitBone() = utils::StringPool::Get(("pelvis"));
									else if (num < 60 && num > 50)
										ret->hitBone() = utils::StringPool::Get(("r_hip"));
									else if (num < 50 && num > 40)
										ret->hitBone() = utils::StringPool::Get(("r_foot"));
									else if (num < 40 && num > 30)
										ret->hitBone() = utils::StringPool::Get(("spine1"));
									else if (num < 30 && num > 20)
										ret->hitBone() = utils::StringPool::Get(("l_hand"));
									else if (num < 20 && num > 10)
										ret->hitBone() = utils::StringPool::Get(("r_upperarm"));
									else if (num < 10)
										ret->hitBone() = utils::StringPool::Get(("l_knee"));
									else
										ret->hitBone() = utils::StringPool::Get(("spine4"));
								}
								else if (VarSs::combat::hitbox == 4) {
									int num = rand() % 100;

									if (num > 90)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 90 && num > 80)
										ret->hitBone() = utils::StringPool::Get(("neck"));
									else if (num < 80 && num > 70)
										ret->hitBone() = utils::StringPool::Get(("l_clavicle"));
									else if (num < 70 && num > 60)
										ret->hitBone() = utils::StringPool::Get(("pelvis"));
									else if (num < 60 && num > 50)
										ret->hitBone() = utils::StringPool::Get(("r_hip"));
									else if (num < 50 && num > 40)
										ret->hitBone() = utils::StringPool::Get(("r_foot"));
									else if (num < 40 && num > 30)
										ret->hitBone() = utils::StringPool::Get(("spine1"));
									else if (num < 30 && num > 20)
										ret->hitBone() = utils::StringPool::Get(("l_hand"));
									else if (num < 20 && num > 10)
										ret->hitBone() = utils::StringPool::Get(("r_upperarm"));
									else if (num < 10)
										ret->hitBone() = utils::StringPool::Get(("l_knee"));
									else
										ret->hitBone() = utils::StringPool::Get(("spine4"));
								}
								else if (VarSs::combat::hitbox == 5) {
									int num = rand() % 100;

									if (num > 90)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 90 && num > 80)
										ret->hitBone() = utils::StringPool::Get(("neck"));
									else if (num < 80 && num > 70)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 70 && num > 60)
										ret->hitBone() = utils::StringPool::Get(("pelvis"));
									else if (num < 60 && num > 50)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 50 && num > 40)
										ret->hitBone() = utils::StringPool::Get(("r_foot"));
									else if (num < 40 && num > 30)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 30 && num > 20)
										ret->hitBone() = utils::StringPool::Get(("spine4"));
									else if (num < 20 && num > 10)
										ret->hitBone() = utils::StringPool::Get(("head"));
									else if (num < 10)
										ret->hitBone() = utils::StringPool::Get(("spine4"));
									else
										ret->hitBone() = utils::StringPool::Get(("head"));
								}
							}

						}
					}
				}
			}
			return ret;

		}
		void LaunchProjectile_hk(BaseProjectile* self)
		{
			// if (VarSs::combat::instakill && GetAsyncKeyState(VarSs::keys::instakill)) {

			   //Vector3 o = LocalPlayer::Entity()->transform()->position();

			   //float desyncTime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
			   //if (self->class_name_hash() == STATIC_CRC32("BaseProjectile")) {
			   //	int ammo = self->primaryMagazine()->contents();
			   //	if (ammo <= 0) return self->LaunchProjectile();

			   //	float f = desyncTime / (self->repeatDelay() * 0.9f);
			   //	int z = (int)f;

			   //	for (size_t i = 0; i < (z > 9 ? 9 : (z < 0 ? 0 : z)); i++)
			   //		if (self->primaryMagazine()->contents() > 0)
			   //		{
			   //			self->LaunchProjectile();
			   //			self->primaryMagazine()->contents()--;
			   //			self->UpdateAmmoDisplay();
			   //			self->ShotFired();
			   //			self->DidAttackClientside();
			   //		}			
			   //		if (z <= 0)
			   //			self->LaunchProjectile();

			   //	LocalPlayer::Entity()->SendClientTick();
			   //	return;				
			   //}
			// }
			return self->LaunchProjectile();
		}

		void hk_DoFirstPersonCamera(PlayerEyes* a1, Component* cam) {
			if (!a1 || !cam) return;
			Original_DoFirstPersonCamera_hk(a1, cam);
			if (VarSs::combat::manipulator) {

				Vector3 re_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
				cam->transform()->set_position(re_p);
			}
		}
	}
	void Hk_TryToMove(ItemIcon* a1) {
		Original_TryToMove_hk(a1);
		if (VarSs::misc::fast_loot) {
			if (a1->queuedForLooting())
			{
				a1->RunTimedAction();
			}
		}
	}
}

void UpdateVelocity_hk(PlayerWalkMovement* self) {

	self->capsule()->set_radius(0.44f);

	Vector3 vel = self->TargetMovement();

	if (!self->flying()) {

		if (VarSs::misc::omnidirectional_sprinting) {
			float max_speed = (self->swimming() || self->Ducking() > 0.5) ? 1.7f : 5.5f;
			if (vel.length() > 0.f) {

				Vector3 target_vel = Vector3(vel.x / vel.length() * max_speed, vel.y, vel.z / vel.length() * max_speed);
				self->TargetMovement() = target_vel;
			}
		}
	}

	if (!self->flying()) {

		if (VarSs::combat::manipulator_stop) {
			if (GetAsyncKeyState(VarSs::keys::manipulated_key)) {
				float max_speed = (self->swimming() || self->Ducking() > 0.5) ? 1.7f : 5.5f;
				if (vel.Length() > 0.f) {
					self->TargetMovement() = (Vector3());
				}
			}
		}

	}
	if (!self->flying()) {

		if (VarSs::misc::speedhack) {
			if (GetAsyncKeyState(VarSs::keys::speedkey)) {
				float max_speed = (self->swimming() || self->Ducking() > 0.5) ? VarSs::misc::speedfloat : VarSs::misc::speedfloat;
				if (vel.length() > 0.f) {

					Vector3 target_vel = Vector3(vel.x / vel.Length() * max_speed, vel.y, vel.z / vel.Length() * max_speed);
					self->TargetMovement() = target_vel;
				}
			}
		}
	}
	return original_updatevelos(self);
}

void HandleJumping_hk(PlayerWalkMovement* a1, ModelState* state, bool wantsJump, bool jumpInDirection = false) {
	if (VarSs::misc::fly_auto_stopper)
	{
		if (HFlyhack >= (HMaxFlyhack - 2.7f)
			|| VFlyhack >= (VMaxFlyhack - 1.7f))
			return original_jumpup(a1, state, false, jumpInDirection);
	}
	if (VarSs::misc::better_jump) {
		if (!wantsJump) return;
		a1->grounded() = (a1->climbing() = (a1->sliding() = false));
		state->set_ducked(false);
		a1->jumping() = true;
		state->set_jumped(true);
		a1->jumpTime() = Time::time();
		a1->ladder() = nullptr;
		Vector3 curVel = a1->body()->velocity();
		a1->body()->set_velocity({ curVel.x, 10, curVel.z });
		return;
	}
	return original_jumpup(a1, state, wantsJump, jumpInDirection);
}
int hitsound_volume;

void DoHitNotify(BaseCombatEntity* entity, HitInfo* info, Projectile* prj) {

	/*if (entity->is_death()) {
		VarSs::players::healthsend0 = true;
		VarSs::players::healthsend1 = false;
	}*/

	if (entity->IsPlayer()) {

		if (VarSs::players::healthbar) {
			VarSs::players::healthsend0 = false;
			VarSs::players::healthsend1 = true;
		}

		if (VarSs::misc::hit_notification) {
			ImGui::Notification({ ImGuiToastType_Info, 4000,  "Hit for damage!" });
		}

		/*if (VarSs::misc::hitmarker) {
			dd1aw::Capsule(info, entity, 0.40f, 2 * 0.40f, Color(VarSs::colors::marker.x, VarSs::colors::marker.y, VarSs::colors::marker.z, 1), 0.01f, false);
		}*/

		if (VarSs::misc::custom_hitsound) {

			if (VarSs::misc::custom_hitsound_type == 0) {
				PlaySoundA((LPCSTR)Carti, NULL, SND_MEMORY | SND_ASYNC);
				return;
			}
			if (VarSs::misc::custom_hitsound_type == 1) {
				PlaySoundA((LPCSTR)PlugTRAP, NULL, SND_MEMORY | SND_ASYNC);
				return;
			}
			if (VarSs::misc::custom_hitsound_type == 2) {
				PlaySoundA((LPCSTR)Buble, NULL, SND_MEMORY | SND_ASYNC);
				return;
			}
			if (VarSs::misc::custom_hitsound_type == 3) {
				PlaySoundA((LPCSTR)Kabanus, NULL, SND_MEMORY | SND_ASYNC);
				return;
			}
		}
		std::string _name(CStringA(reinterpret_cast<BasePlayer*>(entity)->_displayName()));
		std::string string;
		float damage = info->damageTypes()->Total();
		hitmarker::d_indicator_t DmgIndicator;
		DmgIndicator.damage = damage;
		DmgIndicator.player = reinterpret_cast<BasePlayer*>(entity);
		DmgIndicator.hit_box = head;
		DmgIndicator.erase_time = Time::time() + 3.2;
		DmgIndicator.initialized = false;
		hitmarker::get().d_indicators.push_back(DmgIndicator);
		hitmarker::get().add_hit(hitmarker_t(Time::time(), damage, head, reinterpret_cast<BasePlayer*>(entity)->get_bone_pos(head)));
	}
	return original_dohitnotify(entity, info, prj);
}
void OnLand_hk(BasePlayer* ply, float vel) {
	if (!LocalPlayer::Entity()->HasPlayerFlag(PlayerFlags::Connected)) return ply->OnLand(vel);
	if (!VarSs::misc::no_fall)
		ply->OnLand(vel);
}

void hk_(void* Function, void** Original, void* Detour) {
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED) {
		return;
	}
	MH_CreateHook(Function, Detour, Original);
	MH_EnableHook(Function);
}

void hk__() {
	hk_((void*)METHOD("Assembly-CSharp::BaseProjectile::LaunchProjectile(): Void"), (void**)&BaseProjectile::LaunchProjectile_, hk::combat::LaunchProjectile_hk);
	hk_((void*)METHOD("Rust.Data::ModelState::set_flying(Boolean): Void"), (void**)&original_setflying, hk::misc::SetFlying);
	hk_((void*)METHOD("Assembly-CSharp::BaseCombatEntity::DoHitNotify(HitInfo): Void"), (void**)&original_dohitnotify, DoHitNotify);
	//if (DetourAttach(&(PVOID&)Globals::Functions::Original_OnAttacked, Hooks::hk_OnAttacked) != NO_ERROR) return false;
	hk_((void*)METHOD("Assembly-CSharp::Projectile::Update(): Void"), (void**)&original_Update, hk::exploit::ProjectileUpdate_hk);
	hk_((void*)METHOD("Assembly-CSharp::FlintStrikeWeapon::DoAttack(): Void"), (void**)&FlintStrikeWeapon::DoAttack_, hk::combat::DoAttack_hk);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::CanAttack(): Boolean"), (void**)&original_canattack, hk::combat::CanAttack);
	//hk_((void*)METHOD("Assembly-CSharp::BasePlayer::SendClientTick(): Void"), (void**)&BasePlayer::SendClientTick_, hk::misc::SendClientTick);
	hk_((void*)METHOD("Assembly-CSharp::AimConeUtil::GetModifiedAimConeDirection(Single,Vector3,Boolean): Vector3"), (void**)&original_aimconedirection, hk::combat::GetModifiedAimConeDirection);
	hk_((void*)METHOD("Facepunch.Console::ConsoleSystem::Run(Option,String,Object[]): String"), (void**)&original_consolerun, hk::misc::Run);
	hk_((void*)METHOD("Assembly-CSharp::ItemModProjectile::GetRandomVelocity(): Single"), (void**)&ItemModProjectile::GetRandomVelocity_, hk::combat::GetRandomVelocity);
	hk_((void*)METHOD("Assembly-CSharp::HeldEntity::AddPunch(Vector3,Single): Void"), (void**)&original_addpunch, hk::combat::AddPunch);
	hk_((void*)METHOD("UnityEngine.CoreModule::UnityEngine::Vector3::MoveTowards(Vector3,Vector3,Single): Vector3"), (void**)&Vector3_::MoveTowards_, hk::combat::MoveTowards_hk);
	hk_((void*)METHOD("Assembly-CSharp::Projectile::DoMovement(Single): Void"), (void**)&original_domovement, hk::exploit::DoMovement);
	hk_((void*)METHOD("Assembly-CSharp::Projectile::DoHit(HitTest,Vector3,Vector3): Boolean"), (void**)&original_dohit, hk::combat::DoHit);
	hk_((void*)METHOD("Assembly-CSharp-firstpass::TOD_Sky::UpdateAmbient(): Void"), (void**)&original_updateambient, hk::misc::UpdateAmbient);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::ClientInput(InputState): Void"), (void**)&original_clientinput, hk::misc::ClientInput);
	hk_((void*)METHOD("Assembly-CSharp::PlayerWalkMovement::UpdateVelocity(): Void"), (void**)&original_updatevelos, UpdateVelocity_hk);
	hk_((void*)METHOD("Assembly-CSharp::PlayerWalkMovement::HandleJumping(ModelState,Boolean,Boolean): Void"), (void**)&original_jumpup, HandleJumping_hk);
	hk_((void*)METHOD("Assembly-CSharp::BasePlayer::OnLand(Single): Void"), (void**)&BasePlayer::OnLand_, OnLand_hk);
	hk_((void*)METHOD("Assembly-CSharp::PlayerEyes::get_BodyLeanOffset(): Vector3"), (void**)&Original_BodyLeanOffset, hk::combat::hk_BodyLeanOffset);
	hk_((void*)METHOD("Assembly-CSharp::BaseMountable::EyePositionForPlayer(BasePlayer,Quaternion): Vector3"), (void**)&Original_EyePositionForPlayer, hk::combat::hk_EyePositionForPlayer);
	hk_((void*)METHOD("Assembly-CSharp::PlayerEyes::DoFirstPersonCamera(Camera): Void"), (void**)&Original_DoFirstPersonCamera_hk, hk::combat::hk_DoFirstPersonCamera);

}