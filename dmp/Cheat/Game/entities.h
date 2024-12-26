#include <random>
#pragma comment(lib, "Urlmon.lib")
using namespace otherEsp;

float dfc(BasePlayer* player) {
	Vector2 ScreenPos;
	if (!utils::w2s(player->get_bone_pos(head), ScreenPos)) return 1000.f;
	if (!player) return 1000.f;

	return screen_center.distance_2d(ScreenPos);
}

ImColor get_color(BasePlayer* player) {
	static float Alpha = 150;
	static bool Tick = false;
	static float Speed = VarSs::players::rgbfillboxspeed;

	if (!Tick || Alpha != 150)
	{
		Tick = false;
		if (!(Alpha >= 150))
			Alpha += Speed;
		else if (Alpha >= 150)
			Tick ^= 1;
		if (Alpha >= 150)
		{
			Alpha = -150;
		}
	}

	const char* _text;
	static float r = 0.50f, g = 0.00f, b = 0.50f;
	static int cases = 0;
	switch (cases)
	{
	case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
	case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
	case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
	case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
	default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
	}
	if (player->HasPlayerFlag(PlayerFlags::Sleeping)) {
		return ImColor(VarSs::visible::box_color[0], VarSs::visible::box_color[1], VarSs::visible::box_color[2]);
	}

	if (!player->playerModel()->isNpc()) {
		if (LocalPlayer::Entity()->is_teammate((uintptr_t)player))
			return ImColor(VarSs::colors::team_color[0], VarSs::colors::team_color[1], VarSs::colors::team_color[2]);
		else {
			if (player->is_target())
			if (VarSs::esp::esp_type == 0) {
			return ImColor(VarSs::visible::box_color[0], VarSs::visible::box_color[1], VarSs::visible::box_color[2]);
			}			   
			if (VarSs::esp::esp_type == 0) {
				return ImColor(VarSs::visible::box_color[0], VarSs::visible::box_color[1], VarSs::visible::box_color[2]);
			}			
		}
	}
	else {
		if (!player->is_target())
			if (VarSs::esp::esp_type == 0) {
				return ImColor(VarSs::colors::npc_box_color[0], VarSs::colors::npc_box_color[1], VarSs::colors::npc_box_color[2]);
			}		   
		else
			if (VarSs::esp::esp_type == 0) {
			return ImColor(VarSs::visible::box_color[0], VarSs::visible::box_color[1], VarSs::visible::box_color[2]);
			}			
	}
}
void ent_loop() {
	float FOV = VarSs::combat::fov, CurFOV;
	auto entityList = BaseNetworkable::clientEntities()->entityList();
	auto local = LocalPlayer::Entity();
	if (local == nullptr) {
		target_player = nullptr;
		return;
	}
	if (!entityList) {
		target_player = nullptr;
		return;
	}
	if (entityList->vals->size <= 1) {
		target_player = nullptr;
		return;
	}
	if (dfc(target_player) > VarSs::combat::fov) {
		target_player = nullptr;
	}

	//if (VarSs::visuals::radar_) {
	//	radar::radar_logic(ObjectClass, Entity, buff);
	//}
	static int margin = 0;

	if (entityList) {		
	

		int w = 200, h = 150;
		if (target_player)
		{
			if (VarSs::players::TargetBelt && target_player != NULL && !target_player->playerModel()->isNpc() && target_player->IsValid() && !show)
			{
				belt::belt_tab_mov(Vector2(w, h + 25.0f));
				g_Render->FilledRect(belt::pos.x, belt::pos.y, w, 20, ImColor(7, 7, 9, 255), 5.f, ImDrawFlags_RoundCornersTop);
				g_Render->FilledRect(belt::pos.x, belt::pos.y + 20.0f, w, h, ImColor(17, 17, 19, 255), 5.f, ImDrawFlags_RoundCornersBottom);
				//g_Render->FilledRect(belt::pos.x, belt::pos.y + 118, w, 3, ImColor(color_ui[0], color_ui[1], color_ui[2]), 5.f, ImDrawFlags_RoundCornersTop);
				std::string _name(CStringA(target_player->_displayName()));
				g_Render->DrawString(belt::pos.x + 6.0f, belt::pos.y + 3, ImColor(255, 255, 255, 255), render2::none, 2, 10, _name.c_str());
				if (target_player) {
					auto list = target_player->inventory()->containerBelt()->itemList();
					if (list) {
						if (list->size) {
							int y = 15;
							for (int i = 0; i < list->size; i++) {
								auto item = (Item*)list->get(i);
								if (!item)
									continue;

								ImColor col = item->uid() == target_player->clActiveItem() ? ImColor(color_ui[0], color_ui[1], color_ui[2]) : ImColor(255, 255, 255);
								std::string string;
								char buf[256] = { 0 }; sprintf_s(buf, ("Item Belt: %ls [x%d]"), item->info()->displayName()->english(), item->amount());
								string += buf;
								g_Render->DrawString(belt::pos.x + 7.0f, belt::pos.y + 10.0f + y, col, render2::none, 2, 10, string.c_str());

								y += 15;
							}
						}
					}
				}
			}
			if (VarSs::players::TargetWear && target_player != NULL && !target_player->playerModel()->isNpc() && target_player->IsValid() && !show)
			{
				wear::wear_tab_mov(Vector2(w, h + 25.0f));
				g_Render->FilledRect(wear::pos.x, wear::pos.y, w, 20, ImColor(7, 7, 9, 255), 5.f, ImDrawFlags_RoundCornersTop);
				g_Render->FilledRect(wear::pos.x, wear::pos.y + 20.0f, w, h, ImColor(17, 17, 19, 255), 5.f, ImDrawFlags_RoundCornersBottom);
				std::string _name(CStringA(target_player->_displayName()));
				g_Render->DrawString(wear::pos.x + 6.0f, wear::pos.y + 3, ImColor(255, 255, 255, 255), render2::none, 2, 10, _name.c_str());
				if (target_player) {
					auto list = target_player->inventory()->containerWear()->itemList();
					if (list) {
						if (list->size) {
							int y = 15;
							for (int i = 0; i < list->size; i++) {
								auto item = (Item*)list->get(i);
								if (!item)
									continue;

								ImColor col = item->uid() == target_player->clActiveItem() ? ImColor(color_ui[0], color_ui[1], color_ui[2]) : ImColor(255, 255, 255);
								std::string string;
								char buf[256] = { 0 }; sprintf_s(buf, ("Item Wear: %ls [x%d]"), item->info()->displayName()->english(), item->amount());
								string += buf;
								g_Render->DrawString(wear::pos.x + 7.0f, wear::pos.y + 10.0f + y, col, render2::none, 2, 10, string.c_str());

								y += 15;
							}
						}
					}
				}
			}			
		}	
		for (int i = 0; i < entityList->vals->size; i++) {
			uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			auto entity = *reinterpret_cast<BaseEntity**>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
			if (!Entity) continue;
			uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
			if (!Object) continue;
			uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
			if (!ObjectClass) continue;

			pUncStr name = read(ObjectClass + 0x60, pUncStr); if (!name) continue;
			char* buff = name->stub;

			if (!reinterpret_cast<BaseEntity*>(Entity)->IsValid()) { continue; }
			if (!reinterpret_cast<Component*>(Entity)->gameObject()) { continue; }
			if (strstr(buff, ("Local"))) { continue; }

			if (reinterpret_cast<BaseCombatEntity*>(Entity)->IsPlayer()) {
				BasePlayer* Player = (BasePlayer*)Entity;
				if (Player->lifestate() == BaseCombatEntity::LifeState::Dead) continue;
				ImColor col = get_color(Player);
				ImColor color = ImColor(0.f, 0.f, 0.f, 1.f);

				if (!Player->playerModel()->isNpc()) {
					if (VarSs::players::skeleton)

						Skeleton(Player, col);
					ESP(Player, col);
					OOF(Player, col);
				}
				else {
					if (VarSs::npc::skeleton)
						Skeleton(Player, col);
				}

				if (VarSs::misc::recycler) {
					if (entity->_class_name_hash() == STATIC_CRC32("Recycler") && entity->transform()->position().distance(LocalPlayer::Entity()->get_bone_pos(l_foot)) < 5.5) {
						entity->_ServerRPC(("SVSwitch"));						
					}
				}

				if (Player->get_bone_pos(head).x == 0 || Player->get_bone_pos(head).y == 0 || Player->get_bone_pos(head).z == 0) continue;

				if (VarSs::combat::ignore_sleepers && Player->HasPlayerFlag(PlayerFlags::Sleeping)) continue;
				if (VarSs::combat::ignore_npc && Player->playerModel()->isNpc()) continue;
				if (VarSs::combat::ignore_team && LocalPlayer::Entity()->is_teammate((uintptr_t)Player)) continue;

				if (Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), Player->get_bone_pos(head)) > 350) continue;

				if (dfc(Player) < VarSs::combat::fov) {
					if (target_player == nullptr)
						target_player = Player;
					else
						if (dfc(target_player) > dfc(Player))
							target_player = Player;
				}
		
			}
			if (reinterpret_cast<BaseCombatEntity*>(Entity)->ClassNameHash() == STATIC_CRC32("BaseHelicopter")) {
				BaseEntity* helicopter = (BaseEntity*)Entity;
				if (helicopter) {
					/*float health = reinterpret_cast<BaseCombatEntity*>(Entity)->health();
					float maxhealth = reinterpret_cast<BaseCombatEntity*>(Entity)->maxHealth();*/
					Vector3 pos = helicopter->transform()->position();
					Vector2 screenPos;
					if (utils::w2s(pos, screenPos)) {
						if (VarSs::visuals::patrol_heli) {
							std::string string;
							char buf[256] = { 0 }; sprintf_s(buf, ("PatrolHeli"), (int)Math::Distance_3D(LocalPlayer::Entity()->get_bone_pos(head), pos));
							string += buf;
							g_Render->DrawString(screenPos.x, screenPos.y, ImColor(255, 255, 255), render2::outline | render2::centered_x, 2, 10, string.c_str());

							/*g_Render->FilledRect(screenPos.x - 30, screenPos.y + 13, 60, 6, ImColor(0, 0, 0), 0, 0);
							g_Render->FilledRect(screenPos.x - 30, screenPos.y + 13, 60 * (health / maxhealth), 6, ImColor(0, 255, 0), 0, 0);
							g_Render->Rect(screenPos.x - 30, screenPos.y + 13, 60, 6, ImColor(0, 0, 0), 0, 0);*/
						}
						/*if (health > 0) {
							if ((FOV > (CurFOV = GetFovHeli(pos)))) {
								FOV = CurFOV; InGame::stor::closestHeli = (uintptr_t)helicopter;
							}
						}*/
					}
				}
			}
			otherEsp::bradley_(Entity, buff);
			otherEsp::corpse(Entity, buff);
			otherEsp::tc(Entity, buff);
			otherEsp::sleepingbag(Entity, buff);
			otherEsp::bed(Entity, buff);
			//otherEsp::stash(Entity, buff);
			//otherEsp::hackablecrate(Entity, buff);
			otherEsp::world(Entity, buff);
			if (VarSs::ores::show_collectables) {
				miscvis(Entity, buff, VarSs::ores::stone, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("stone-collectable.prefab"), (L"Stone Collectable"), ImColor(128, 128, 128), texture_cache_byte.get_texture(stonenode, sizeof(stonenode)),ImVec2(35, 35));
				miscvis(Entity, buff, VarSs::ores::sulfur, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("sulfur-collectable.prefab"), (L"Sulfur Collectable"), ImColor(255, 215, 0), texture_cache_byte.get_texture(sulfurnode, sizeof(sulfurnode)), ImVec2(35, 35));
				miscvis(Entity, buff, VarSs::ores::metal, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("metal-collectable.prefab"), (L"Metal Collectable"), ImColor(139, 69, 19), texture_cache_byte.get_texture(metalnode, sizeof(metalnode)), ImVec2(35, 35));
			}
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::ores::stone, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("stone-ore.prefab"), (L"Stone Ore"), ImColor(128, 128, 128), texture_cache_byte.get_texture(stonenode, sizeof(stonenode)), ImVec2(35, 35));
			miscvis(Entity, buff, VarSs::ores::sulfur, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("sulfur-ore.prefab"), (L"Sulfur Ore"), ImColor(255, 215, 0), texture_cache_byte.get_texture(sulfurnode, sizeof(sulfurnode)), ImVec2(35, 35));
			miscvis(Entity, buff, VarSs::ores::metal, VarSs::ores::show_distance, VarSs::ores::draw_distance, ("metal-ore.prefab"), (L"Metal Ore"), ImColor(139, 69, 19), texture_cache_byte.get_texture(metalnode, sizeof(metalnode)), ImVec2(35, 35));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::crates::elite, VarSs::visuals::crates::show_distance, VarSs::visuals::crates::draw_distance, ("crate_elite.prefab"), (L"Elite Crate"), ImColor(46, 139, 87), texture_cache_byte.get_texture(elitecrate, sizeof(elitecrate)), ImVec2(23,11));
			miscvis(Entity, buff, VarSs::visuals::crates::military, VarSs::visuals::crates::show_distance, VarSs::visuals::crates::draw_distance, ("crate_normal.prefab"), (L"Military Crate"), ImColor(1, 68, 33), texture_cache_byte.get_texture(militarycrate, sizeof(militarycrate)), ImVec2(23,12));
			miscvis(Entity, buff, VarSs::visuals::crates::supply, VarSs::visuals::crates::show_distance, VarSs::visuals::crates::draw_distance, ("supply_drop.prefab"), (L"Airdrop"), ImColor(0, 139, 139), texture_cache_byte.get_texture(airdrop, sizeof(airdrop)), ImVec2(14,15));
			miscvis(Entity, buff, VarSs::visuals::crates::heli, VarSs::visuals::crates::show_distance, VarSs::visuals::crates::draw_distance, ("heli_crate.prefab"), (L"Heli Crate"), ImColor(1, 50, 32), texture_cache_byte.get_texture(elitecrate, sizeof(elitecrate)), ImVec2(23,11));
			miscvis(Entity, buff, VarSs::visuals::crates::bradley, VarSs::visuals::crates::show_distance, VarSs::visuals::crates::draw_distance, ("bradley_crate.prefab"), (L"Bradley Crate"), ImColor(173, 255, 47), texture_cache_byte.get_texture(bradleycrate, sizeof(bradleycrate)), ImVec2(23,11));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::vehicles::minicopter, VarSs::visuals::vehicles::show_distance, VarSs::visuals::vehicles::draw_distance, ("minicopter.entity.prefab"), (L"Minicopter"), ImColor(0, 0, 255), texture_cache_byte.get_texture(minicopter, sizeof(minicopter)), ImVec2(19,11));
			miscvis(Entity, buff, VarSs::visuals::vehicles::scrapheli, VarSs::visuals::vehicles::show_distance, VarSs::visuals::vehicles::draw_distance, ("scraptransporthelicopter.prefab"), (L"Scrap Heli"), ImColor(0, 0, 139), texture_cache_byte.get_texture(scrapheli, sizeof(scrapheli)), ImVec2(22,11));
			miscvis(Entity, buff, VarSs::visuals::vehicles::boat, VarSs::visuals::vehicles::show_distance, VarSs::visuals::vehicles::draw_distance, ("rowboat.prefab"), (L"Boat"), ImColor(173, 216, 230), texture_cache_byte.get_texture(rowboat, sizeof(rowboat)), ImVec2(21,7));
			miscvis(Entity, buff, VarSs::visuals::vehicles::rhib, VarSs::visuals::vehicles::show_distance, VarSs::visuals::vehicles::draw_distance, ("rhib.prefab"), (L"RHIB"), ImColor(224, 255, 255), texture_cache_byte.get_texture(rhib, sizeof(rhib)), ImVec2(24,9));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::turrets::auto_turret, VarSs::visuals::turrets::show_distance, VarSs::visuals::turrets::draw_distance, ("autoturret_deployed.prefab"), (L"Auto Turret"), ImColor(255, 0, 0), texture_cache_byte.get_texture(autoturret, sizeof(autoturret)), ImVec2(12,12));
			miscvis(Entity, buff, VarSs::visuals::turrets::flame_turret, VarSs::visuals::turrets::show_distance, VarSs::visuals::turrets::draw_distance, ("flameturret.deployed.prefab"), (L"Flame Turret"), ImColor(255, 0, 0), texture_cache_byte.get_texture(autoturret, sizeof(autoturret)), ImVec2(12,12));
			miscvis(Entity, buff, VarSs::visuals::turrets::shotgun_turret, VarSs::visuals::turrets::show_distance, VarSs::visuals::turrets::draw_distance, ("guntrap.deployed.prefab"), (L"Shotgun Trap"), ImColor(255, 0, 0), texture_cache_byte.get_texture(shotguntrap, sizeof(shotguntrap)), ImVec2(16,14));
			miscvis(Entity, buff, VarSs::visuals::turrets::landmine, VarSs::visuals::turrets::show_distance, VarSs::visuals::turrets::draw_distance, ("landmine.prefab"), (L"Landmine"), ImColor(138, 43, 226));
			miscvis(Entity, buff, VarSs::visuals::turrets::sam_site, VarSs::visuals::turrets::show_distance, VarSs::visuals::turrets::draw_distance, ("sam_site_turret_deployed.prefab"), (L"SAM Site"), ImColor(255, 0, 0), texture_cache_byte.get_texture(samsite, sizeof(samsite)), ImVec2(13,14));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::other::hemp, VarSs::visuals::other::show_distance, VarSs::visuals::other::draw_distance, ("hemp-collectable.prefab"), (L"Hemp"), ImColor(50, 205, 50), texture_cache_byte.get_texture(hemp, sizeof(hemp)), ImVec2(45 , 45));

			miscvis(Entity, buff, VarSs::visuals::other::bodybag, VarSs::visuals::other::show_distance, VarSs::visuals::other::draw_distance, ("item_drop_backpack.prefab"), (L"Bodybag"), ImColor(139, 0, 139));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::base::boxes, VarSs::visuals::base::show_distance, VarSs::visuals::base::draw_distance, ("box.wooden.large.prefab"), (L"Box"), ImColor(188, 143, 143));
			// ---------------------------------------------------------
			miscvis(Entity, buff, VarSs::visuals::animals::bear, VarSs::visuals::animals::show_distance, VarSs::visuals::animals::draw_distance, ("bear.prefab"), (L"Bear"), ImColor(139, 69, 19), texture_cache_byte.get_texture(bear, sizeof(bear)), ImVec2(17,12));
			miscvis(Entity, buff, VarSs::visuals::animals::wolf, VarSs::visuals::animals::show_distance, VarSs::visuals::animals::draw_distance, ("wolf.prefab"), (L"Wolf"), ImColor(119, 136, 153));
			miscvis(Entity, buff, VarSs::visuals::animals::pig, VarSs::visuals::animals::show_distance, VarSs::visuals::animals::draw_distance, ("boar.prefab"), (L"Pig"), ImColor(255, 136, 77), texture_cache_byte.get_texture(hog, sizeof(hog)), ImVec2(20,12));
			miscvis(Entity, buff, VarSs::visuals::animals::chicken, VarSs::visuals::animals::show_distance, VarSs::visuals::animals::draw_distance, ("chicken.prefab"), (L"Chicken"), ImColor(173, 255, 47), texture_cache_byte.get_texture(chicken, sizeof(chicken)), ImVec2(12,13));
			miscvis(Entity, buff, VarSs::visuals::animals::deer, VarSs::visuals::animals::show_distance, VarSs::visuals::animals::draw_distance, ("horse.prefab"), (L"Horse"), ImColor(244, 164, 96), texture_cache_byte.get_texture(horse, sizeof(horse)), ImVec2(13,12));
		}

		/*targeting shit*/
		if (target_player != nullptr) {
	
			if (target_player->playerModel()->isNpc() && VarSs::combat::ignore_npc) {
				target_player = NULL;
			}
			if (target_player->HasPlayerFlag(PlayerFlags::Sleeping) && VarSs::combat::ignore_sleepers) {
				target_player = NULL;
			}
			if (LocalPlayer::Entity()->is_teammate((uintptr_t)target_player) && VarSs::combat::ignore_team) {
				target_player = NULL;
			}
			if (target_player->health() <= 0) {
				target_player = NULL;
			}
		}

		if (InGame::stor::closestHeli != NULL) {
			if (reinterpret_cast<BaseEntity*>(InGame::stor::closestHeli)->IsDestroyed()) {
				InGame::stor::closestHeli = NULL;
			}
			if (!reinterpret_cast<Component*>(InGame::stor::closestHeli)->gameObject()) {
				InGame::stor::closestHeli = NULL;
			}
		}
		/*if (LocalPlayer::Entity() != nullptr) {
			if (vars::combat::aimbot && !LocalPlayer::Entity()->is_teammate(reinterpret_cast<BasePlayer*>(target_player))) {
				if (target_player) {
					if (GetAsyncKeyState(vars::keys::aimkey)) {
						do_aimbot(reinterpret_cast<BasePlayer*>(target_player));
					}
				}
			}
		}*/
	}
}