-------------------------------------------
-- [GLOBAL]
-------------------------------------------
add_rules("mode.debug", "mode.release")
set_languages("cxx20")
set_strip("debug")
set_symbols("debug")
set_warnings("all")
add_cxxflags("/wd 4267")
set_toolchains("msvc", {vs = "2019"})

if is_mode("release") then
  set_optimize("fastest")
elseif is_mode("debug") then
  add_defines("DEBUG")
  set_optimize("none")
end

add_includedirs(
  "src",
  "deps/glm-0.9.9.8/include",
  "deps/eastl-3.17.06/include",
  "deps/spdlog-1.9.2/include",
  "deps/boost-1.77/include",
  "deps/vulkan-1.2.198.1/include",
  "deps/SPIRV-Cross-moltenvk_1.1.5/include",
  "deps/tinygltf-2.5.0/include",
  "deps/dxc-1.7.0/include",
  "deps/ktx-4.0.0/include"
)

if is_arch("x64") and is_os("windows") then
    add_linkdirs(
      "deps/eastl-3.17.06/win64",
      "deps/vulkan-1.2.198.1/win64",
      "deps/boost-1.77/win64",
      "deps/ktx-4.0.0/win64"
    )
    add_defines("PLATFORM_WIN64")
end

-------------------------------------------
-- [GLOBAL_END]
-------------------------------------------
rule("ecs_generation")
  set_extensions(".ecs.cpp")
  on_build_file(function (target, sourcefile, opt)
        import("core.project.depend")
        import("utils.progress")

        includes = target:get("includedirs")
        includesArg = ""
        for _, i in ipairs(includes) do
          includesArg = format("%s -I %s", includesArg, i)
        end

        os.mkdir(target:targetdir())

        depend.on_changed(function ()
            codegen = "src/engine/ecs/codegen/ecs_generator.py"
            cmd = format("python %s %s %s", codegen, includesArg, sourcefile)
            os.vrun(cmd)
            progress.show(opt.progress, "${color.build.object}ecs generation %s", sourcefile)
        end, {files = sourcefile})
    end)

target("spirv-cross")
  set_kind("static")
  add_files("deps/SPIRV-Cross-moltenvk_1.1.5/src/*.cpp")
  set_languages("cxx11")

target("aster-ecs")
  set_kind("object")
  add_rules("ecs_generation")
  add_files("src/aster/ecs/*.ecs.cpp")

target("aster")
  set_kind("binary")
  add_files("src/aster/main.cpp",
            "src/aster/ecs/*gen.cpp"
           )
  add_deps("aster-ecs", "engine", "engine-materials")
  add_ldflags("-WHOLEARCHIVE:engine-materials", {force = true})
  set_suffixname("-$(mode)")
  set_targetdir("/game/aster")

target("engine-materials")
  set_kind("static")
  add_files("src/engine/materials/**.cpp")

target("engine-assets")
  set_kind("static")
  add_files("src/engine/assets/**.cpp")

target("engine-datablock")
  set_kind("static")
  add_files("src/engine/datablock/**.cpp")

target("engine-ecs")
  set_kind("static")
  add_files("src/engine/ecs/**.cpp")

target("engine-blk")
  set_kind("static")
  add_files("src/engine/gapi/**.cpp")

target("engine-input")
  set_kind("static")
  add_files("src/engine/input/**.cpp")

target("engine-gapi")
  set_kind("static")
  add_files("src/engine/render/**.cpp")

target("engine-render")
  set_kind("static")
  add_files("src/engine/render/**.cpp")

target("engine-scene")
  set_kind("static")
  add_files("src/engine/scene/**.cpp")

target("engine-utils")
  set_kind("static")
  add_files("src/engine/utils/**.cpp")

target("engine")
  set_kind("static")
  add_files("src/engine/*.cpp")
  add_links("user32",
            "EASTL",
            "vulkan-1",
            "ktx")
  add_deps("spirv-cross",
           "engine-assets",
           "engine-blk",
           "engine-datablock",
           "engine-ecs",
           "engine-gapi",
           "engine-input",
           "engine-render",
           "engine-scene",
           "engine-utils")

target("blk-test")
  set_kind("binary")
  add_deps("engine")
  add_files("src/test/datablock/main.cpp")

target("shaders_compiler")
  set_kind("binary")
  add_links("dxcompiler")
  add_files("src/shaders_compiler/*.cpp")
  add_deps("engine")
  set_suffixname("-$(mode)")
  set_targetdir("/tools")
