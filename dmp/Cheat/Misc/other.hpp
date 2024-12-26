#include <array>
#define M_PI_2 1.57079632679489661923f

void Circle3D1(Vector3 pos, Color clr, float radius, float penis = 0) {
    Vector3 prev = Vector3(pos.x + radius, pos.y, pos.z);
    for (float num = 0.f; num < 6.2831855f; num += 0.05f) {

        float val_x = cos(num) * radius;
        float val_z = sin(num) * radius;
        Vector3 newPos = Vector3(pos.x + val_x, pos.y + penis, pos.z + val_z);

        dd1aw::Line(prev, newPos, clr, 0.01f, false, false);

        prev = newPos;
    }
}

inline void dynamic_vectors(std::vector<Vector3>& re, float max = 2.0f, float maxy = 2.0f, int numPoints = 100)
{
    float max_radius = max; 

    for (int i = 0; i < numPoints; ++i) {
        float vec1 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
        float vec2 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;

        float x = max_radius * sinf(vec2) * cosf(vec1);
        float y = maxy * sinf(vec2) * sinf(vec1);
        float z = max_radius * cosf(vec2);

        re.emplace_back(x, y, 0.f);
        re.emplace_back(-0.f, -y, -z);
        re.emplace_back(x, 0.f, z);

        re.emplace_back(0.f, y, 0.f);
        re.emplace_back(0.f, -y, 0.f);

        re.emplace_back(-x, 0.f, 0.f);
        re.emplace_back(x, 0.f, 0.f);

        re.emplace_back(0.f, 0.f, z);
        re.emplace_back(0.f, 0.f, -z);
    }
}
//inline void dynamic_vectors(std::vector<Vector3>& re, float mm_max = 3.0f)
//{
//
//    for (float eye = 2.0f; eye < VarSs::combat::points_dync_generate; ++eye) {
//        float VEC1 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
//        float VEC2 = static_cast<float>((rand)()) / static_cast<float>(RAND_MAX) * 2.0f * M_PI;
//        Vector3 vec_vis_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
//        Vector3 vec_vis_p_target = VarSs::stuff::best_target;
//
//        float vec_x = 2.0f * sin(VEC2 * VEC1) * cos(2.0f);
//        float vec_y = 2.0f * sin(VEC2 * VEC1) * cos(2.0f);
//        float vec_z = 2.0f * sin(VEC2 * VEC1) * cos(2.0f);
//
//        // vec_auto
//        re.emplace_back(-vec_x, vec_y, vec_z);
//        re.emplace_back(vec_x, -vec_y, vec_z);
//        re.emplace_back(vec_x, vec_y, -vec_z);
//
//        // vec_push
//        re.push_back(Vector3(vec_x, vec_y, -vec_z));
//        re.push_back(Vector3(vec_x, -vec_y, vec_z));
//        re.push_back(Vector3(-vec_x, vec_y, vec_z));
//
//        if (utils::IsVisible(vec_vis_p, vec_vis_p_target) && nullptr) {
//            re.emplace_back(-vec_x * 1.0f, -vec_y * 1.25f, vec_z * 1.5f);
//        }
//        if (utils::LineOfSight(vec_vis_p, vec_vis_p_target) && nullptr) {
//            re.push_back(Vector3(vec_x * 0.5f, -vec_y * 0.25f, -vec_z * 1.5f));
//        }
//    }
//}
namespace other {
    Vector3 m_manipulate = Vector3::Zero();
    void find_manipulate_angle() {
        Vector3 re_p = LocalPlayer::Entity()->transform()->position() + LocalPlayer::Entity()->transform()->up() * (PlayerEyes::EyeOffset().y + LocalPlayer::Entity()->eyes()->viewOffset().y);
        Vector3 nulls = Vector3::Zero();
        if (VarSs::stuff::best_target != Vector3(0, 0, 0)) {    
            if (utils::LineOfSight(re_p, VarSs::stuff::best_target)) {
                m_manipulate = Vector3::Zero();
                return;
            }
        }
        else {
            if (utils::LineOfSight(re_p, reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head))) {
                m_manipulate = Vector3::Zero();
                return;
            }
        }
        float desyncTime = (Time::realtimeSinceStartup() - LocalPlayer::Entity()->lastSentTickTime()) - 0.03125 * 3;
        float mm_max_eye = (0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * LocalPlayer::Entity()->MaxVelocity()) - 0.05f;
        float calculating_max_eye_pos = ((0.1f + ((desyncTime + 2.f / 60.f + 0.125f) * 1.5f) * LocalPlayer::Entity()->MaxVelocity()));
        float max_v_multicplier = 3.5f;
        float v_mm_max_eye = mm_max_eye / max_v_multicplier;

        std::vector<Vector3> random = {};

        dynamic_vectors(random, mm_max_eye, 2.0f, VarSs::combat::points_dync_generate);

        for (auto s : random) {

            Vector3 point = re_p + s;
           
            //dd1aw::Sphere(point, 0.05f, Color(1, 1, 1, 255), 0.05f, false);
                      
            if (VarSs::combat::Player_Circles)
            {
                auto transform = reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(spine1);
                Circle3D1(transform + Vector3(0, mm_max_eye / 5 - 1.5, 0), Color(VarSs::colors::player_circles_color.x * 255, VarSs::colors::player_circles_color.y * 255, VarSs::colors::player_circles_color.z * 255, 255), 1.5f, 0.f);               
            }

            if (VarSs::combat::ManipulatorRadios)
            {
                Vector3 actual_desync_pos = LocalPlayer::Entity()->get_bone_pos(spine1);
                float step = (M_PI * 2.0f) / 72;               

                for (float a = 0; a < (M_PI * 2.0f); a += step)
                {
                    Vector3 start(mm_max_eye * cos(a) + actual_desync_pos.x, actual_desync_pos.y, mm_max_eye * sin(a) + actual_desync_pos.z);
                    Vector3 end(mm_max_eye * cos(a + step) + actual_desync_pos.x, actual_desync_pos.y, mm_max_eye * sin(a + step) + actual_desync_pos.z);

                    Vector3 max_manip_start(2.0f * cos(a) + actual_desync_pos.x, actual_desync_pos.y, 2.0f * sin(a) + actual_desync_pos.z);
                    Vector3 max_manip_end(2.0f * cos(a + step) + actual_desync_pos.x, actual_desync_pos.y, 2.0f * sin(a + step) + actual_desync_pos.z);

                    dd1aw::Line(start, end, Color(VarSs::colors::MPRadius.x * 255, VarSs::colors::MPRadius.y * 255, VarSs::colors::MPRadius.z * 255, 255), 0.f, false, true);
                }
            }

            if (!utils::LineOfSight(point, re_p)) continue;
            if (VarSs::stuff::best_target != Vector3(0, 0, 0)) {
                if (!utils::LineOfSight(VarSs::stuff::best_target, point))
                    continue;
            }
            else {
                if (!utils::LineOfSight(reinterpret_cast<BasePlayer*>(target_player)->get_bone_pos(head), point))
                    continue;
            }
            VarSs::stuff::eyemani = point;
            nulls = s;
            break;
        }
        if (nulls.empty()) {
            m_manipulate = Vector3::Zero();
            return;
        }
        m_manipulate = nulls;
    }
}