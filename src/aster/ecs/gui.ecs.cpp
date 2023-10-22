#include <engine/console/cmd.h>
#include <engine/console/console.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/template_utils.h>
#include <engine/events.h>
#include <engine/gui/gui.h>
#include <engine/input/input.h>
#include <engine/math.h>
#include <engine/render/debug/render.h>
#include <engine/time.h>
#include <engine/types.h>
#include <engine/utils/collision.h>
#include <engine/utils/math.h>
#include <engine/work_cycle/camera.h>
// ECS_SYSTEM()
// static void camera_rotation(
//   const float2& camera_rotations
// )
// {
//   const float2 pos = Engine::Input::manager.getAnalogStatus(str_hash("MouseMove"));
//   Engine::gui::manager.setMouseCursorPos(pos);
// }

// ECS_EVENT_SYSTEM()
// static void input_handler(const ButtonActionInputEvent& evt)
// {
//   if (evt.action == str_hash("MouseClick"))
//     Engine::gui::manager.setMouseClickState(evt.status == Engine::Input::ButtonStatus::Press);
// }

ECS_EVENT_SYSTEM()
static void system_init(ecs::OnEntityCreated& evt, const float2 test_float2)
{
  volatile bool f = false;
}

ECS_SYSTEM()
static void system_test_multiple_templates(const float& test_float, const string& test_str, const float2 test_float2, const float3 test_float3)
{
  volatile const float fl = test_float;
  volatile const string str = test_str;
  volatile const float2 fl2 = test_float2;
  volatile const float3 fl3 = test_float3;
}

ECS_SYSTEM()
static void system_test_default_str(const string& default_str)
{
  volatile const string str = default_str;
}

ECS_SYSTEM()
static void system_test_test_float2(const float2 test_float2)
{
  volatile const float2 fl2 = test_float2;
}

ECS_SYSTEM()
static
void system_src_tmpl(const float fl_data, const string& txt_data)
{
  volatile const float fl = fl_data;
}

ECS_SYSTEM()
static
void system_dst_tmpl(const float fl_data, const string& txt_data2)
{
  volatile const float fl = fl_data;
}

ecs::EntityId srcEid;

void create_src(eastl::span<string_view> args)
{
  auto& ecs = ecs::get_registry();
  srcEid = ecs.createEntity("src_tmpl", {});
}

void recreate_src(eastl::span<string_view> args)
{
  auto& ecs = ecs::get_registry();
  ecs::EntityComponents init;
  init["fl_data"] = 0.5f;
  init["txt_data2"] = string{"Mbbbbbb"};
  srcEid = ecs.recreateEntity(srcEid, "dst_tmpl", std::move(init));
}

void add_subt(eastl::span<string_view>args)
{
  auto& ecs = ecs::get_registry();
  auto eTmpl = ecs.getEntityTemplateName(srcEid);
  console::clog(fmt::format("adding template subt to entity with tmpl {}", eTmpl));
  srcEid = ecs::add_sub_template(srcEid, "subt");
  console::clog(fmt::format("new template: {}", ecs.getEntityTemplateName(srcEid)));
}

void remove_subt(eastl::span<string_view>args)
{
  auto& ecs = ecs::get_registry();
  auto eTmpl = ecs.getEntityTemplateName(srcEid);
  console::clog(fmt::format("removing template subt from entity with tmpl {}", eTmpl));
  srcEid = ecs::remove_sub_template(srcEid, "subt");
  console::clog(fmt::format("new template: {}", ecs.getEntityTemplateName(srcEid)));
}

ECS_SYSTEM()
static
void system_flag(const bool flag)
{
  const volatile bool f = flag;
}

void create_pawn(eastl::span<string_view> args)
{
  auto& ecs = ecs::get_registry();
  ecs.createEntity("pawn");
}

ECS_EVENT_SYSTEM()
static
void system_on_pawn_creation(const ecs::OnEntityCreated& evt, const float pawn_pos)
{
  ecs::EntityComponents init;
  init["pawn_eid"] = evt.eid;

  console::clog(fmt::format("created pawn with id:{}", evt.eid.getId()));

  auto& ecs = ecs::get_registry();
  ecs.createEntity("controller", std::move(init));
}

ECS_QUERY()
static
void query_pawn(const ecs::EntityId eid, eastl::function<void(const float pawn_pos)>);

ECS_SYSTEM()
static
void system_controller(const ecs::EntityId pawn_eid, bool controller_flag)
{
  volatile const bool f = controller_flag;

  volatile float fv = 0.0f;
  query_pawn(pawn_eid, [&](const float v){
    fv = v;
  });
}

ECS_EVENT_SYSTEM()
static
void system_on_moving_decal_creation(const ecs::OnEntityCreated& evt, float3& center_pos,  const float3& pos)
{
  center_pos = pos;
}

ECS_SYSTEM()
static
void moving_decal_controller(const bool moving_decal_flag, const float3& center_pos, float3& pos)
{
  float t = Engine::Time::get_sec_since_start();
  pos.y = center_pos.y + 0.3 * std::cos(t);
}

ECS_DESCRIBE_QUERY(query_camera, (
  const float3& pos,
  const float2& camera_rotations,
  const float3& forward))

static
void draw_line_at_camera_pos(eastl::span<string_view> args)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    Engine::dbg::draw_line(pos, pos + forward * float3(100.0), float3(1.0, 0.0, 0.0), 10.0);
  });
}

static
void draw_aabbs_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2&, const float3& forward){
    float3 cubeCenter = pos + forward * float3(2.0);
    Engine::dbg::draw_aabb(cubeCenter, float3{1,1,0.1}, float4(1.0, 0.0, 0.0, 0.3), 20.0);
    Engine::dbg::draw_aabb(cubeCenter + float3(0.0, 0.0, 2.0), float3{1,1,0.1}, float4(0.0, 1.0, 0.0, 0.6), 20.0);
    Engine::dbg::draw_aabb(cubeCenter + float3(0.0, 0.0, 4.0), float3{1,1,0.1}, float4(0.0, 0.0, 1.0, 0.8), 20.0);
  });
}

static
void draw_line_plane_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    
    const float3 plPos = pos + forward * float3(2.0);
    Utils::Plane p{plPos, glm::normalize(float3{0.0, 1.0, 1.0})};
    Engine::dbg::draw_line_plane(p, plPos, 0.4, float3(1.0, 0.0, 0.0), 10.0, true);
  });
}

static
void draw_plane_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    
    const float3 plPos = pos + forward * float3(2.0);
    Utils::Plane p{plPos, glm::normalize(float3{0.0, 1.0, 1.0})};
    Engine::dbg::draw_plane(p, plPos, 0.4, float4(1.0, 1.0, 1.0, 0.5), 10.0, true);
  });
}

static
void draw_sphere_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    
    const float3 center = pos + forward * float3(2.0);
    Utils::Sphere sp{center, 1.0};
    Engine::dbg::draw_line_sphere(sp, float3(0.0, 0.0, 1.0), 20.0);
  });
}

static
void spawn_collision_tests(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    
    const float3 center = pos + forward * float3(2.0);
    ecs::EntityComponents init;
    init["center"] = pos + forward * 2.0f;
    ecs::get_registry().createEntity("CollisionTests", std::move(init));
  });
}

ECS_EVENT_SYSTEM()
void tick_collision_tests(Engine::OnGameTick&, const bool collision_test_tag, const float3& center)
{
  float3 cameraForward;

  query_camera([&cameraForward](const float3& pos, const float2& rotation, const float3& forward){
    cameraForward = forward;
  });

  //sphere-plane
  {
    Utils::Sphere sp{center + float3{0.0, 0.0, 1.0} * 2.0f, 0.3f };
    const float3 plPos = sp.center + float3{0.0, 1.0, 0.0};
    Utils::Plane pl{plPos, glm::normalize(float3{1.0, -1.0, 0.0})};
    sp.center += (float3{0.0, 2.0, 0.0} * std::abs(std::sin(Engine::Time::get_sec_since_start())));

    Engine::dbg::draw_plane(pl, plPos, 2.0, float4(1.0, 1.0, 1.0, 0.8f), 0.0f, true);

    if (Utils::test_intersection(pl, sp))
      Engine::dbg::draw_line_sphere(sp, float3{1.0, 0.0, 0.0}, 0.0f);
    else
      Engine::dbg::draw_line_sphere(sp, float3{0.0, 0.0, 1.0}, 0.0f);
  }

  //sphere-sphere
  {
    Utils::Sphere s1{center + float3{0.0, 0.0, 1.0} * 2.0f + float3{-1.0, 0.0, 0.0}, 0.3f };
    Utils::Sphere s2 = s1;
    s2.r = 0.4;
    s1.center += (float3{0.0, 1.0, 0.0} * std::sin(Engine::Time::get_sec_since_start()));

    Engine::dbg::draw_line_sphere(s1, float3{0.0, 1.0, 0.0}, 0.0f);
    if (Utils::test_intersection(s1,s2))
      Engine::dbg::draw_line_sphere(s2, float3{1.0, 0.0, 0.0}, 0.0f);
    else
      Engine::dbg::draw_line_sphere(s2, float3{0.0, 0.0, 1.0}, 0.0f);
  }

  //3 planes
  {
    const float3 planesOrigin = center + float3{0.0, 0.0, 1.0} * 2.0f + float3(2.0, 0.0,0.0);
    Utils::Plane p1{planesOrigin, glm::normalize(float3{1.0, 0.0, 0.0})};
    const float3 p2Origin = planesOrigin + float3{0.0, 0.1, 0.0} * std::sin(Engine::Time::get_sec_since_start());
    Utils::Plane p2{p2Origin, glm::normalize(float3{1.0, 1.0, 0.0})};
    Utils::Plane p3{planesOrigin, glm::normalize(float3{0.0, 0.0, 1.0})};

    const float3 intPoint = Utils::calc_intersect_point(p1,p2,p3).value();

    Engine::dbg::draw_aabb(intPoint, {0.05, 0.05, 0.05}, float4{1.0, 1.0, 0.0, 1.0}, 0.0);
    Engine::dbg::draw_plane(p1, planesOrigin, 1.0, float4{1.0, 0.0, 0.0, 0.5}, 0.0f);
    Engine::dbg::draw_plane(p2, p2Origin, 1.0, float4{0.0, 1.0, 0.0, 0.5}, 0.0f);
    Engine::dbg::draw_plane(p3, planesOrigin, 1.0, float4{0.0, 0.0, 1.0, 0.5}, 0.0f);
  }

  //sphere- half space
  {
    const float3 spCenter = center + float3{-3.0f, 0.0f, 1.0f};
    Utils::Sphere sp{spCenter + float3{0.0f, 0.0f, 1.0f} * 2.0f, 0.3f };
    const float3 plPos = sp.center + float3{0.0f, 1.0f, 0.0f};
    Utils::Plane pl{plPos, glm::normalize(float3{1.0f, -1.0f, 0.0f})};
    sp.center += (float3{0.0f, 2.0f, 0.0f} * std::abs(std::sin(Engine::Time::get_sec_since_start())));

    Engine::dbg::draw_plane(pl, plPos, 2.0f, float4(1.0f, 1.0f, 1.0f, 0.8f), 0.0f, true);

    if (Utils::test_half_space(pl, sp))
      Engine::dbg::draw_line_sphere(sp, float3{1.0f, 0.0f, 0.0f}, 0.0f);
    else
      Engine::dbg::draw_line_sphere(sp, float3{0.0f, 0.0f, 1.0f}, 0.0f);
  }

  //sphere-frustum
  {
    const float3 spCenter = center + float3{-3.0f, -2.0f, 1.0f};

    float4x4 view = math::look_at(spCenter + glm::normalize(float3{-1.0f, -1.0f, 0.0f}) * 2.0f, spCenter);
    float4x4 proj = math::perspective(45.0f, 1980.0f / 1024.0, 1.0f, 3.0f);

    Utils::Frustum frustum{proj * view};
    Engine::dbg::draw_frustum(frustum, float4{1.0f, 1.0f, 1.0f, 0.3f}, 0.0f);

    Utils::Sphere sp{spCenter + float3{-5.0, -5.0, -5.0} * std::abs(std::sin(Engine::Time::get_sec_since_start())), 0.2f};
    if (Utils::test_intersection(frustum, sp))
      Engine::dbg::draw_line_sphere(sp, float3{1.0, 0.0, 0.0}, 0.0f);
    else
      Engine::dbg::draw_line_sphere(sp, float3{0.0, 0.0, 1.0}, 0.0f);
  }
}

static
void draw_frustum_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    float4x4 view = math::look_at(pos + forward * 2.0f, pos);
    float4x4 proj = math::perspective(45.0f, 1980.0f / 1024.0, 1.0f, 5.0f);

    Utils::Frustum frustum{proj * view};
    Utils::Sphere camPoint{pos, 0.05f};

    Engine::dbg::draw_line_sphere(camPoint, float3(1.0f, 0.0f, 0.0f), 20.0f);
    Engine::dbg::draw_frustum(frustum, float4(1.0f, 1.0f, 1.0f, 0.3f), 20.0f, true);
  });
}

static
void draw_clustered_frustum_at_camera_pos(eastl::span<string_view>)
{
  query_camera([](const float3& pos, const float2& rotation, const float3& forward){
    const int3 N = {4,4,8};

    const float3 camPos = pos;
    const float near = 1.0f;
    const float far = 10.0f;

    const float4x4 view = math::look_at(camPos + forward * 2.0f, camPos);
    const float4x4 proj = math::perspective_inv_z(45.0f, 1980.0f / 1024.0, near, far);
    Utils::Frustum frustum{proj * view};

    const math::FarPlanePoints fpWorld =
          math::get_far_plane_points_world_space(proj, view);

    const float3 farCenter = (fpWorld.leftBot + fpWorld.rightTop) * 0.5f;
    const float3 originToFarCenterDir = glm::normalize(farCenter - camPos);
    const float3 nearCenter = camPos + originToFarCenterDir * near;

    Utils::Plane xPlanes[N.x+1];
    Utils::Plane yPlanes[N.y+1];
    Utils::Plane zPlanes[N.z+1];

    const float3 up = glm::normalize(fpWorld.leftTop - fpWorld.leftBot);
    const float3 right = glm::normalize(fpWorld.rightTop - fpWorld.leftTop);

    for (int i = 0; i <= N.x; ++i)
    {
      const float t = (float)i / (float)N.x;
      const float3 p = glm::mix(fpWorld.leftTop, fpWorld.rightTop, t);
      
      const float3 e2 = up;
      const float3 e3{p-camPos};
      const float3 N = glm::normalize(glm::cross(e2, e3));

      xPlanes[i] = Utils::Plane{p, N};
    }

    for (int i = 0; i <= N.y; ++i)
    {
      const float t = (float)i / (float)N.y;
      const float3 p = glm::mix(fpWorld.leftBot, fpWorld.leftTop, t);
      
      const float3 e1 = right;
      const float3 e3{p-camPos};
      const float3 N = glm::normalize(glm::cross(e3, e1));

      yPlanes[i] = Utils::Plane{p, N};
    }

    for (int i = 0; i <= N.z; ++i)
    {
      const float t = (float)i / (float)N.z;
      const float3 p = glm::mix(nearCenter, farCenter, t);
      
      const float3 N = originToFarCenterDir;
      zPlanes[i] = Utils::Plane{p, N};
    }
    
    eastl::vector<Utils::Frustum> frustums;
    frustums.resize(N.x * N.y * N.z);

    for (size_t z = 0; z < N.z; ++z)
    for (size_t y = 0; y < N.y; ++y)
    for (size_t x = 0; x < N.x; ++x)
    {
      const size_t idx = x + y * N.x + z * N.x * N.y;
      frustums[idx] = {
        xPlanes[x], xPlanes[x+1].inv(), yPlanes[y], yPlanes[y+1].inv(), zPlanes[z], zPlanes[z+1].inv()
      };
    }

    for (size_t z = 0; z < N.z; ++z)
    for (size_t y = 0; y < N.y; ++y)
    for (size_t x = 0; x < N.x; ++x)
    {
      const size_t idx = x + y * N.x + z * N.x * N.y;
      Engine::dbg::draw_frustum(frustums[idx], float4{(float)(x+1) / (float)N.x , (float)(y+1) / (float)N.y, (float)(z+1) / (float)N.z, 0.1}, 200.0f);
    }
  });
}



CONSOLE_CMD("draw_line", 0, 0, draw_line_at_camera_pos);
CONSOLE_CMD("draw_line_plane", 0, 0, draw_line_plane_at_camera_pos);
CONSOLE_CMD("draw_plane", 0, 0, draw_plane_at_camera_pos);
CONSOLE_CMD("draw_sphere", 0, 0, draw_sphere_at_camera_pos);
CONSOLE_CMD("draw_aabb", 0, 0, draw_aabbs_at_camera_pos);
CONSOLE_CMD("draw_frustum", 0, 0, draw_frustum_at_camera_pos);
CONSOLE_CMD("draw_cl_frustum", 0, 0, draw_clustered_frustum_at_camera_pos);
CONSOLE_CMD("test_collision", 0, 0, spawn_collision_tests);
CONSOLE_CMD("create_src", 0, 0, create_src);
CONSOLE_CMD("recreate", 0, 0, recreate_src);
CONSOLE_CMD("add", 0, 0, add_subt);
CONSOLE_CMD("del", 0, 0, remove_subt);
CONSOLE_CMD("pawn", 0, 0, create_pawn);