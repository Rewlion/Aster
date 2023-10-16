#include <engine/console/cmd.h>
#include <engine/console/console.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/template_utils.h>
#include <engine/events.h>
#include <engine/gui/gui.h>
#include <engine/input/input.h>
#include <engine/render/debug/render.h>
#include <engine/time.h>
#include <engine/types.h>
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
    Engine::dbg::draw_aabb(cubeCenter, float3{1,1,1}, float4(0, 1.0, 0.0, 0.1), 20.0);

    Engine::dbg::draw_aabb(cubeCenter, float3{0.5,2,0.5}, float4(0, 0.0, 1.0, 0.1), 20.0);
  });
}

CONSOLE_CMD("draw_line", 0, 0, draw_line_at_camera_pos);
CONSOLE_CMD("draw_aabb", 0, 0, draw_aabbs_at_camera_pos);
CONSOLE_CMD("create_src", 0, 0, create_src);
CONSOLE_CMD("recreate", 0, 0, recreate_src);
CONSOLE_CMD("add", 0, 0, add_subt);
CONSOLE_CMD("del", 0, 0, remove_subt);
CONSOLE_CMD("pawn", 0, 0, create_pawn);