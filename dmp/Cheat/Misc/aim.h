#include <map>

void PredictionWeapon(Vector3 local, Vector3& target, Vector3 targetvel, float velonity) {
	BasePlayer* TargetPlayer = reinterpret_cast<BasePlayer*>(target_player);
	auto base_projectile = LocalPlayer::Entity()->GetHeldEntity<BaseProjectile>();
	if (base_projectile == nullptr)
		return;
	float distance_to_travel;
	static Type* type = Type::GetType(("ItemModProjectile, Assembly-CSharp"));
	if (type == nullptr)
		return;

	auto mag = base_projectile->primaryMagazine();
	if (mag == nullptr)
		return;

	auto ammo = mag->ammoType();
	if (ammo == nullptr)
		return;

	auto itemModProjectile = ammo->GetComponent<ItemModProjectile>(type);
	if (itemModProjectile == nullptr)
		return;

	Projectile* projectile = itemModProjectile->projectileObject()->Get()->GetComponent<Projectile>(Type::Projectile());

	if (projectile == nullptr)
		return;

	/*if (projectile) {
		auto PVS = base_projectile->projectileVelocityScale();

		if (vars::weapons::fast_bullets) {
			PVS = (1.500f) * base_projectile->projectileVelocityScale();
		}
		else
		{
			PVS = (1.000f) * base_projectile->projectileVelocityScale();
		}
	}*/


	float m_flBulletSpeed = (itemModProjectile->projectileVelocity() * (base_projectile->projectileVelocityScale() * (VarSs::weapons::fast_bullets ? 1.499f : 1.0f)));
	float distance = target.distance(LocalPlayer::Entity()->eyes()->position());
	float bullet_time = distance / m_flBulletSpeed;
	//const float m_flTimeStep = 0.005f;
	const float m_flTimeStep = 0.001f;
	float m_flYTravelled{}, m_flYSpeed{}, m_flBulletTime{}, m_flDivider{};

	//float m_flDistanceTo = fVrom.distance(aimpoint);

	for (distance_to_travel = 0.f; distance_to_travel < distance;)
	{
		//float speed_modifier = (aidsware::ui::get_bool(xorstr_("fast bullets")) ? 1.48f : 1.0f) - m_flTimeStep * projectile->drag();
		float speed_modifier = 1.0f - m_flTimeStep * projectile->drag();
		m_flBulletSpeed *= speed_modifier;

		if (m_flBulletSpeed <= 0.f || m_flBulletSpeed >= 10000.f || m_flYTravelled >= 10000.f || m_flYTravelled < 0.f)
			break;

		if (m_flBulletTime > 10.f)
			break;

		m_flYSpeed += (9.8f * projectile->gravityModifier()) * m_flTimeStep;
		m_flYSpeed *= speed_modifier;

		distance_to_travel += m_flBulletSpeed * m_flTimeStep;
		m_flYTravelled += m_flYSpeed * m_flTimeStep;
		m_flBulletTime += m_flTimeStep;
	}
	//float Dist = Math::Distance_3D(target, local);



	//Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * velonity;// 0.8
	Vector3 velocity = targetvel * velonity;
	if (velocity.y > 0.f)
		velocity.y /= 3.25f;
	Vector3 PredictVel = velocity * m_flBulletTime;


	target += PredictVel;
	target.y += m_flYTravelled;

}

float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!utils::w2s(Entity->get_bone_pos(Bone), ScreenPos)) return 2000.f;
	return Math::Distance_2D(Vector2(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2), ScreenPos);
}
float GetFovHeli(Vector3 pos) {
	Vector2 ScreenPos;
	if (!utils::w2s(pos, ScreenPos)) return 2000.f;
	return Math::Distance_2D(Vector2(VarSs::stuff::ScreenWidth / 2, VarSs::stuff::ScreenHeight / 2), ScreenPos);
}
//float GetBulletSpeed() {
//	Item* active = LocalPlayer::Entity()->GetActiveWeapon();
//	Weapon tar = active->Info();
//	int ammo = active->LoadedAmmo();
//	if (ammo == 0) return vars::weapons::fast_bullets ? tar.ammo[0].speed * 1.3 + vars::stuff::testFloat : tar.ammo[0].speed + vars::stuff::testFloat;
//	for (Ammo am : tar.ammo) {
//		for (int id : am.id) {
//			if (id == ammo) {
//				return vars::weapons::fast_bullets ? am.speed * 1.3 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
//			}
//		}
//		if (am.id[0] == 0) return vars::weapons::fast_bullets ? am.speed * 1.3 + vars::stuff::testFloat : am.speed + vars::stuff::testFloat;
//	}
//	return vars::weapons::fast_bullets ? 250.f * 1.3 + vars::stuff::testFloat : 250.f + vars::stuff::testFloat;
//}
//void do_aimbot(BasePlayer* player) {
//	if (!LocalPlayer::Entity() || !player) { return; }
//	if (!LocalPlayer::Entity()->GetActiveWeapon() || player->IsDestroyed()) { return; }
//	bool long_neck = vars::misc::long_neck && GetAsyncKeyState(vars::keys::longneck);
//	Vector3 local = long_neck ? LocalPlayer::Entity()->get_bone_pos(head) + Vector3(0, 1.15, 0) : LocalPlayer::Entity()->get_bone_pos(head);
//	Vector3 target = vars::combat::bodyaim ? player->get_bone_pos(spine1) : player->get_bone_pos(head);
//	a::Prediction(local, target, player->newVelocity(), GetBulletSpeed(), GetGravity(LocalPlayer::Entity()->GetActiveWeapon()->LoadedAmmo()));
//	Vector2 Offset = Math::CalcAngle(local, target) - LocalPlayer::Entity()->viewangles();
//	Math::Normalize(Offset.y, Offset.x);
//	Vector2 AngleToAim = LocalPlayer::Entity()->viewangles() + Offset;
//	if (vars::combat::smooth) {
//		StepConstant(AngleToAim);
//	}
//	Math::Normalize(AngleToAim.y, AngleToAim.x);
//	LocalPlayer::Entity()->set_viewangles(AngleToAim);
//}