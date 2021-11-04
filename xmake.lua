-------------------------------------------
-- [GLOBAL]
-------------------------------------------
add_rules("mode.debug", "mode.release")
set_languages("cxx20")
set_strip("debug")
set_symbols("debug")
set_warnings("all")
set_toolchains("msvc", {vs = "2019"})

if is_mode("release") then
  set_optimize("fastest")
elseif is_mode("debug") then
  add_defines("DEBUG")
  set_optimize("none")
end 

add_includedirs(
  "src",
  "deps/entt-3.8.1/include",
  "deps/glfw-3.3.4/include",
  "deps/glm-0.9.9.8/include",
  "deps/eastl-3.17.06/include"
)

if is_arch("x64") and is_os("windows") then
    add_linkdirs(
      "deps/eastl-3.17.06/win64"
    )
end

-------------------------------------------
-- [GLOBAL_END]
-------------------------------------------


target("engine")
  set_kind("static")
  add_files("src/engine/datablock/datablock.cpp",
            "src/engine/datablock/parser/lex.yy.cpp",
            "src/engine/datablock/parser/parser.cpp",
            "src/engine/datablock/parser/parser.tab.cpp")