

namespace lol {
	float LastUpdate = 0.f;
	/*void update_chams() {
		if (Time::realtimeSinceStartup() > LastUpdate + VarSs::players::uptime) {
			reinterpret_cast<void(*)()>(InGame::stor::gBase + CO::RebuildAll)();
			LastUpdate = Time::realtimeSinceStartup();
		}
	}*/

	//void hammerspam1_attack(uintptr_t Held, bool transform) {
	//	if (!target_player || !Held) return;

	//	HitTest* hit = HitTest::New();
	//	auto playerPos = target_player->transform_(head)->position();

	//	if (read(Held + O::AttackEntity::nextAttackTime, float) >= Time::time()) { return; }
	//	if (read(Held + O::AttackEntity::timeSinceDeploy, float) < read(Held + O::AttackEntity::deployDelay, float)) { return; }

	//	//uintptr_t staticHitTest = read(InGame::stor::gBase + CO::HitTest, DWORD64); if (!staticHitTest) return;
	//	uintptr_t newHitTest = il2cpp::il2cpp_object_new(hit); if (!newHitTest) return;

	//	uintptr_t trans; Ray ray = Ray(LocalPlayer::Entity()->get_bone_pos(head), (target_player - LocalPlayer::Entity()->get_bone_pos(head)).Normalized());
	//	if (!target_player) return;
	//	if (transform) {
	//		trans = reinterpret_cast<BasePlayer*>(target_player)->boneTransforms(head);
	//	}
	//	else {
	//		trans = (DWORD64)reinterpret_cast<Object*>(target_player)->transform();
	//	} if (!trans) {
	//		return;
	//	}

	//	hit->MaxDistance() = 1000.f;
	//	hit->HitTransform() = target_player->transform_(head);
	//	hit->AttackRay() = Ray(LocalPlayer::Entity()->eyes()->get_position(), (playerPos - LocalPlayer::Entity()->eyes()->get_position()).Normalized());
	//	hit->DidHit() = true;
	//	hit->HitEntity() = target_player;
	//	hit->HitPoint() = target_player->eyes()->transform()->InverseTransformPoint(target_player->eyes()->transform()->position() + Vector3(Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f)));
	//	hit->HitNormal() = target_player->eyes()->transform()->InverseTransformPoint(target_player->eyes()->transform()->position() + Vector3(Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f), Random::Range(-0.2f, 0.2f)));

	//	if (VarSs::weapons::hammer_spam_type == 0)
	//	{
	//		hit->HitMaterial_() = il2cpp::String::New(("Glass"));
	//	}

	//	else if (VarSs::weapons::hammer_spam_type == 1)
	//	{
	//		hit->HitMaterial_() = il2cpp::String::New(("Water"));
	//	}

	//	if (VarSs::weapons::hammer_spam_type == 2)
	//	{
	//		hit->HitMaterial_() = il2cpp::String::New(("Forest"));
	//	}

	//	if (VarSs::weapons::hammer_spam_type == 3)
	//	{
	//		hit->HitMaterial_() = il2cpp::String::New(("Metal"));
	//	}

	//	reinterpret_cast<void(*)(uintptr_t, float)>(InGame::stor::gBase + CO::StartAttackCooldown)(Held, read(Held + O::AttackEntity::repeatDelay, float));

	//	return reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(InGame::stor::gBase + CO::ProcessAttack)(Held, newHitTest);
	//}
}

float w_last_syringe = 0.f;
BasePlayer* MatrixUpdater;
void TestFlying() {
	
	if (!LocalPlayer::Entity()->OnLadder()) {
		flyhackPauseTime = Mathf::Max(0.f, flyhackPauseTime - Time::deltaTime());
		bool inAir = false;
		float radius = LocalPlayer::Entity()->GetRadius();
		float height = LocalPlayer::Entity()->GetHeight(false);
		Vector3 vector = (LocalPlayer::Entity()->lastSentTick()->position() + LocalPlayer::Entity()->playerModel()->position()) * 0.5f;
		Vector3 vector2 = vector + Vector3(0.f, radius - 2.f, 0.f);
		Vector3 vector3 = vector + Vector3(0.f, height - radius, 0.f);
		float radius2 = radius - 0.05f;
		bool a = Physics::CheckCapsule(vector2, vector3, radius2, 1503731969, QueryTriggerInteraction::Ignore);
		inAir = !a;

		if (inAir) {
			bool flag = false;

			Vector3 vector4 = (LocalPlayer::Entity()->playerModel()->position() - LocalPlayer::Entity()->lastSentTick()->position());
			float num3 = Mathf::Abs(vector4.y);
			float num4 = Misc::Magnitude2D(vector4);
			if (vector4.y >= 0.f) {
				flyhackDistanceVertical += vector4.y;
				flag = true;
			}
			if (num3 < num4) {
				flyhackDistanceHorizontal += num4;
				flag = true;
			}
			if (flag) {
				float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
				float num6 = LocalPlayer::Entity()->GetJumpHeight() + num5;
				if (flyhackDistanceVertical > num6) {
				}
				float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
				float num8 = 5.f + num7;
				if (flyhackDistanceHorizontal > num8) {
				}
			}
		}
		else {
			flyhackDistanceHorizontal = 0.f;
			flyhackDistanceVertical = 0.f;
		}
		float flyhack_forgiveness_interia = 10.0f;
		float flyhack_forgiveness = 1.5f;
		float flyhack_extrusion = 1.85f;
		float num5 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
		float num6 = ((LocalPlayer::Entity()->GetJumpHeight() + num5) * 3);

		VMaxFlyhack = num6;
		if (flyhackDistanceVertical <= (num6)) {
			VFlyhack = flyhackDistanceVertical;
		}
		if (VFlyhack >= VMaxFlyhack)
			VFlyhack = VMaxFlyhack;
		float num7 = Mathf::Max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
		float num8 = ((5.f + num7) * 3);

		HMaxFlyhack = num8;
		if (flyhackDistanceHorizontal <= (num8)) {
			HFlyhack = flyhackDistanceHorizontal;
		}
		if (HFlyhack >= HMaxFlyhack)
			HFlyhack = HMaxFlyhack;
	}
}
void CheckFlyhack() {
	TestFlying();
	float num5 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num6 = LocalPlayer::Entity()->GetJumpHeight() + num5;
	VarSs::stuff::max_flyhack = num6;
	if (flyhackDistanceVertical <= num6) {
		VarSs::stuff::flyhack = flyhackDistanceVertical;
	}

	float num7 = Mathf::Max((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
	float num8 = 5.f + num7;
	VarSs::stuff::max_hor_flyhack = num8;
	if (flyhackDistanceHorizontal <= num8) {
		VarSs::stuff::hor_flyhack = flyhackDistanceHorizontal;
	}
}
