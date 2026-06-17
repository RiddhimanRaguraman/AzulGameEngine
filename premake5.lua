--[[
    AzulGameEngine premake5 build script.
    Generates the Engine or Converter Visual Studio 2022 solution
    to mirror the hand-written .sln/.vcxproj layout 1:1.

    Usage:
        premake5.exe --ws=engine    vs2022
        premake5.exe --ws=converter vs2022
]]

newoption {
    trigger     = "ws",
    value       = "NAME",
    description = "Choose which workspace to generate",
    allowed = {
        { "engine",    "Generate Engine workspace" },
        { "converter", "Generate Converter workspace" }
    }
}

local wsName = _OPTIONS["ws"] or "engine"

------------------------------------------------------------------------
-- Helpers
------------------------------------------------------------------------

-- Mirrors $(SolutionDir)$(Platform)\$(Configuration)\ used by the hand
-- written vcxprojs as the OutDir.
local function outDir() return "%{wks.location}/x64/%{cfg.buildcfg}" end
local function objOut() return "%{wks.location}/x64/%{cfg.buildcfg}/%{prj.name}" end

-- TargetName = $(ProjectName)DebugX64 / $(ProjectName)ReleaseX64
local function applyTargetName()
    filter "configurations:Debug"
        targetname "%{prj.name}DebugX64"
    filter "configurations:Release"
        targetname "%{prj.name}ReleaseX64"
    filter {}
end

-- Common project settings used by every C++ project (libs, tests, apps).
local function applyCommonProject()
    language              "C++"
    cppdialect            "C++17"
    characterset          "MBCS"
    conformancemode       "On"
    forceincludes         { "Framework.h" }
    multiprocessorcompile "On"
    staticruntime         "Off"
    rtti                  "On"
    buildoptions          { "/sdl" }

    defines {
        "BEAUCERON",
        "SOLUTION_DIR=R\"($(SolutionDir))\"",
        "TOOLS_VERSION=R\"($(VCToolsVersion))\"",
        "LOCAL_WORKING_DIR=R\"($(LocalDebuggerWorkingDirectory))\"",
        "WINDOWS_TARGET_PLATFORM=\"$(TargetPlatformVersion)\"",
        "_CONSOLE"
    }

    filter "system:windows"
        systemversion "latest"
        defines       { "WIN32", "_WINDOWS" }

    filter "configurations:Debug"
        defines       { "_DEBUG" }
        symbols       "On"
        optimize      "Off"
        runtime       "Debug"

    filter "configurations:Release"
        defines       { "NDEBUG" }
        symbols       "On"
        optimize      "Full"
        runtime       "Release"
        functionlevellinking "On"
        intrinsics    "On"

    filter {}

    prebuildcommands {
        "if exist \"$(SolutionDir)Azul\" rmdir /S /Q \"$(SolutionDir)Azul\""
    }
end

------------------------------------------------------------------------
-- Workspace
------------------------------------------------------------------------

if wsName == "engine" then
    workspace "Engine"
        startproject "Engine"
elseif wsName == "converter" then
    workspace "Converter"
        startproject "Converter"
end

    location       (os.getcwd())
    configurations { "Debug", "Release" }
    architecture   "x64"

------------------------------------------------------------------------
-- Shared item projects (.vcxitems)
------------------------------------------------------------------------

local function defineSharedItem(name)
    project(name)
        kind     "SharedItems"
        language "C++"
        location ("Shared/" .. name)
        files {
            "Shared/" .. name .. "/**.h",
            "Shared/" .. name .. "/**.hpp",
            "Shared/" .. name .. "/**.cpp",
            "Shared/" .. name .. "/**.cc",
            "Shared/" .. name .. "/**.inc",
            "Shared/" .. name .. "/**.natvis"
        }
        removefiles {
            "Shared/" .. name .. "/**.vcxproj*",
            "Shared/" .. name .. "/**.vcxitems*"
        }
        -- Mirror the on-disk folder layout in Solution Explorer
        vpaths { ["*"] = "Shared/" .. name }

        -- ProtoBuf_pb_items hosts the *.pb.cc/.pb.h files emitted by
        -- the protoc prebuild step. On the first generate-then-build
        -- pass those files don't exist yet, so the **.cc glob above
        -- misses them and the shared project ends up empty (which
        -- makes every *_proto symbol unresolved at link time).
        -- Derive the expected paths from the .proto inputs so the
        -- entries are present regardless of generation order.
        if name == "ProtoBuf_pb_items" then
            for _, protoPath in ipairs(os.matchfiles("Libs/ProtoBuf/src/*.proto")) do
                local base = path.getbasename(protoPath)
                files {
                    "Shared/ProtoBuf_pb_items/" .. base .. ".pb.cc",
                    "Shared/ProtoBuf_pb_items/" .. base .. ".pb.h"
                }
            end
        end
end

local sharedItems
if wsName == "engine" then
    sharedItems = { "Framework_items", "ProtoBuf_pb_items", "ProtoBuf_lib_items" }
else
    sharedItems = { "Framework_items", "ProtoBuf_pb_items", "ProtoBuf_lib_items", "GLTF_items" }
end

-- Engine .sln nests Framework_items / ProtoBuf_pb_items / ProtoBuf_lib_items
-- under a "Shared" solution folder. Converter .sln keeps them at the top.
if wsName == "engine" then
    group "Shared"
end
for _, name in ipairs(sharedItems) do
    defineSharedItem(name)
end
group ""

------------------------------------------------------------------------
-- DLL libraries
------------------------------------------------------------------------

local libDefines = {
    Math      = { exports = "MATH_LIBRARY_EXPORTS",            use = "MATH_USE_DLL"            },
    AnimTime  = { exports = "ANIM_TIME_LIBRARY_EXPORTS",       use = "ANIM_TIME_USE_DLL"       },
    File      = { exports = "FILE_LIBRARY_EXPORTS",            use = "FILE_USE_DLL"            },
    Manager   = { exports = "MANAGER_LIBRARY_EXPORTS",         use = "MANAGER_USE_DLL"         },
    PCSTree   = { exports = "PCSTREE_LIBRARY_EXPORTS",         use = "PCSTREE_USE_DLL"         },
    PugiXml   = { exports = "PUGI_XML_LIBRARY_EXPORTS",        use = "PUGI_XML_USE_DLL"        },
    ProtoBuf  = { exports = "PROTOBUF_ENGINE_LIBRARY_EXPORTS", use = "PROTOBUF_ENGINE_USE_DLL" }
}

local function defineDllLib(name)
    project(name)
        kind     "SharedLib"
        location ("Libs/" .. name)
        targetdir(outDir())
        objdir   (objOut())

        files {
            "Libs/" .. name .. "/src/**.cpp",
            "Libs/" .. name .. "/src/**.h",
            "Libs/" .. name .. "/include/**.h",
            "Libs/" .. name .. "/include/**.cd"
        }
        removefiles {
            "Libs/" .. name .. "/**.vcxproj*",
            "Libs/" .. name .. "/Test/**"
        }

        -- Mirror the on-disk layout (include/, src/) in Solution Explorer
        vpaths { ["*"] = "Libs/" .. name }

        includedirs {
            "Libs/" .. name .. "/include",
            "Shared/Framework_items"
        }

        applyCommonProject()
        applyTargetName()

        warnings "Everything"   -- /Wall (EnableAllWarnings) for libs

        local lib = libDefines[name]
        if lib then
            defines { lib.exports, lib.use }
        end

        filter "configurations:Release"
            floatingpoint "Fast"
        filter {}

        links { "Framework_items" }

        -- PugiXml needs its src dir and the pugixml export macro
        if name == "PugiXml" then
            includedirs { "Libs/PugiXml/src" }
            defines     { "PUGIXML_API=__declspec(dllexport)", "pugixml_shared_EXPORTS" }
        end

        -- Math copies its Keenan-supplied DLLs into the build dir
        if name == "Math" then
            prebuildcommands {
                "copy \"$(ProjectDir)Keenan\\*.*\" \"$(SolutionDir)$(Platform)\\$(Configuration)\""
            }
        end

        -- ProtoBuf: warning level 4, extra includes, protoc step, DLL copy
        if name == "ProtoBuf" then
            warnings "High"     -- /W4 (Level4) to match Engine
            includedirs {
                "Libs/ProtoBuf/src",
                "Libs/Math/include",
                "Shared/ProtoBuf_pb_items",
                "Shared/ProtoBuf_lib_items",
                "Shared/ProtoBuf_lib_items/absl",
                "Shared/ProtoBuf_lib_items/utf8_range"
            }
            defines { "PROTOBUF_USE_DLLS", "ABSL_CONSUME_DLL", "MATH_USE_DLL" }
            links   { "Math", "ProtoBuf_pb_items", "ProtoBuf_lib_items" }

            files  { "Libs/ProtoBuf/src/**.proto" }
            vpaths { ["Proto Files"] = { "**.proto" } }

            prebuildcommands {
                "\"$(SolutionDir)Shared\\ProtoBuf_lib_items\\tools\\protoc.exe\" --cpp_out=.\\..\\..\\Shared\\ProtoBuf_pb_items --proto_path=.\\src .\\src\\*.proto"
            }

            filter "configurations:Debug"
                libdirs { "Shared/ProtoBuf_lib_items/libs/debug" }
                links   { "libprotobuf-lited", "abseil_dll", "libutf8_range", "libutf8_validity" }
                postbuildcommands {
                    "copy \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\debug\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
                }

            filter "configurations:Release"
                libdirs { "Shared/ProtoBuf_lib_items/libs/release" }
                links   { "libprotobuf-lite", "abseil_dll", "libutf8_range", "libutf8_validity" }
                postbuildcommands {
                    "copy \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\release\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
                }

            filter {}
        end
end

local libsForEngine    = { "Math", "AnimTime", "File", "Manager", "PCSTree", "PugiXml", "ProtoBuf" }
local libsForConverter = { "Math", "File", "PugiXml", "ProtoBuf" }
local activeLibs = (wsName == "engine") and libsForEngine or libsForConverter

-- Both solutions nest the DLL libraries under a "Libs" solution folder.
group "Libs"
for _, lib in ipairs(activeLibs) do
    defineDllLib(lib)
end
group ""

------------------------------------------------------------------------
-- Engine workspace projects (tests + engine app)
------------------------------------------------------------------------

if wsName == "engine" then

    ------------------------------------------------------
    -- Library unit-test console apps (Tests solution folder)
    ------------------------------------------------------
    group "Tests"
    local tests = {
        FileTest    = "File",
        MathTest    = "Math",
        PCSTreeTest = "PCSTree",
        PugiXmlTest = "PugiXml"
    }

    for testName, libName in pairs(tests) do
        project(testName)
            kind     "ConsoleApp"
            location ("Libs/" .. libName)
            targetdir(outDir())
            objdir   ("%{wks.location}/x64/" .. testName .. "/%{cfg.buildcfg}")

            -- The hand-written test projects exclude every individual
            -- test-case file and only build the *Group / *Test / main
            -- driver files. Match that pattern.
            files {
                "Libs/" .. libName .. "/Test/main.cpp",
                "Libs/" .. libName .. "/Test/**Group.cpp",
                "Libs/" .. libName .. "/Test/**Test.cpp",
                "Libs/" .. libName .. "/Test/**Test.h",
                "Libs/" .. libName .. "/Test/_UnitTestConfiguration.h"
            }
            -- Keep on-disk layout (Test/) in Solution Explorer
            vpaths { ["*"] = "Libs/" .. libName }

            includedirs {
                "Libs/" .. libName .. "/include",
                "Libs/Math/include",
                "Shared/Framework_items"
            }

            applyCommonProject()
            applyTargetName()

            warnings "Everything"

            defines { libDefines[libName].use }
            if libName ~= "Math" then
                defines { "MATH_USE_DLL" }
            end

            links { libName, "Math", "Framework_items" }
    end
    group ""

    ------------------------------------------------------
    -- Engine application
    ------------------------------------------------------
    project "Engine"
        kind     "WindowedApp"
        location "Engine/src"
        targetdir(outDir())
        objdir   (objOut())
        debugdir "$(SolutionDir)Data"

        files {
            "Engine/src/**.h",
            "Engine/src/**.cpp",
            "Engine/src/**.cd",
            "Engine/shaders/original/**.hlsl",
            "Engine/shaders/original/**.hlsli",
            "Engine/shaders/Compiled/**.h"
        }
        removefiles { "Engine/src/**.vcxproj*" }

        -- Mirror Engine/src/, Engine/shaders/original/, Engine/shaders/Compiled/
        vpaths { ["*"] = "Engine" }

        includedirs {
            "Shared/Framework_items",
            "Shared/ProtoBuf_pb_items",
            "Shared/ProtoBuf_lib_items",
            "Shared/ProtoBuf_lib_items/absl",
            "Shared/ProtoBuf_lib_items/utf8_range",
            "Libs/ProtoBuf/include",
            "Libs/ProtoBuf/src",
            "Libs/Math/include",
            "Libs/AnimTime/include",
            "Libs/File/include",
            "Libs/PCSTree/include",
            "Libs/Manager/include",
            "Libs/PugiXml/include",
            "Engine/shaders/Compiled",
            "Engine/src",
            "Engine/DXTex/include"
        }

        applyCommonProject()

        warnings "High"   -- /W4 (Level4)
        rtti     "Off"

        -- Defines must match the hand-written Engine.vcxproj exactly.
        -- Notably MANAGER_USE_DLL / PCSTREE_USE_DLL / PUGI_XML_USE_DLL
        -- are NOT set: the Engine compiles classes derived from
        -- Manager.h templates locally (TexNodeCompareStrategyEnumName,
        -- SkelCompareStrategyEnumName, etc.). Setting MANAGER_USE_DLL
        -- here flips the template base to __declspec(dllimport), which
        -- propagates to the derived constructors and turns them into
        -- unresolved dllimport symbols at link time.
        defines {
            "MATH_USE_DLL",
            "ANIM_TIME_USE_DLL",
            "FILE_USE_DLL",
            "DIRECTX_TEX_IMPORT",
            "PROTOBUF_USE_DLLS",
            "ABSL_CONSUME_DLL",
            "PROTOBUF_ENGINE_USE_DLLS"
        }

        links {
            "Math", "AnimTime", "File", "Manager", "PCSTree", "PugiXml", "ProtoBuf",
            "Framework_items", "ProtoBuf_pb_items", "ProtoBuf_lib_items",
            "d3d11", "dxgi", "d3dcompiler", "winmm"
        }

        filter "configurations:Debug"
            libdirs { "Shared/ProtoBuf_lib_items/libs/debug" }
            links   { "libprotobuf-lited", "abseil_dll", "libutf8_range", "libutf8_validity" }
            postbuildcommands {
                "copy /Y \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\debug\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
            }

        filter "configurations:Release"
            libdirs { "Shared/ProtoBuf_lib_items/libs/release" }
            links   { "libprotobuf-lite", "abseil_dll", "libutf8_range", "libutf8_validity" }
            linktimeoptimization "On"
            postbuildcommands {
                "copy /Y \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\release\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
            }

        filter {}

        ----------------------------------------------------
        -- HLSL shader compilation: model 5.0, header out,
        -- _d.cso for debug / .cso for release, named by suffix.
        --
        -- The variable name strips the type suffix:
        --   ColorByVertex.Px.hlsl -> g_ColorByVertex_PxShader
        ----------------------------------------------------
        local shaderTypes = { Px = "Pixel", Vx = "Vertex", Cx = "Compute" }
        local shaderDir = "Engine/shaders/original"
        if os.isdir(shaderDir) then
            for _, file in ipairs(os.matchfiles(shaderDir .. "/*.hlsl")) do
                local base   = path.getbasename(file)             -- e.g. ColorByVertex.Px
                local stem, suffix = base:match("(.+)%.(%w+)$")   -- ColorByVertex , Px
                if stem and shaderTypes[suffix] then
                    filter { "files:" .. file }
                        buildaction            "FxCompile"
                        shadermodel            "5.0"
                        shadertype             (shaderTypes[suffix])
                        shadervariablename     ("g_" .. stem .. "_" .. suffix .. "Shader")
                        shaderheaderfileoutput ("../Shaders/Compiled/" .. base .. ".h")

                    filter { "files:" .. file, "configurations:Debug" }
                        shaderobjectfileoutput ("$(OutDir)" .. base .. "_d.cso")

                    filter { "files:" .. file, "configurations:Release" }
                        shaderobjectfileoutput ("$(OutDir)" .. base .. ".cso")

                    filter {}
                end
            end
        end
end

------------------------------------------------------------------------
-- Converter workspace project
------------------------------------------------------------------------

if wsName == "converter" then
    project "Converter"
        kind       "WindowedApp"
        location   "Converter"
        targetdir  (outDir())
        objdir     (objOut())
        entrypoint "mainCRTStartup"
        debugdir   "$(SolutionDir)Data"

        files {
            "Converter/**.h",
            "Converter/**.cpp"
        }
        removefiles { "Converter/**.vcxproj*" }

        -- Match the hand-authored Solution Explorer grouping.
        -- The files all sit flat in Converter/ on disk, but the
        -- original Converter.vcxproj.filters buckets them by role.
        vpaths {
            ["Source Files/GLTF/*"]              = {
                "Converter/GLTF.*", "Converter/GLTF_Helper.*"
            },
            ["Source Files/Mesh/*"]              = {
                "Converter/ConvertMesh.*"
            },
            ["Source Files/Texture/*"]           = {
                "Converter/CreateTexture.*"
            },
            ["Source Files/TinyGLTF_Converter/*"] = {
                "Converter/meshDataConverter.*",
                "Converter/vboDataConverter.*",
                "Converter/textureDataConverter.*"
            },
            ["Source Files/Primitive_Mesh/*"]    = {
                "Converter/CubeMesh.*",
                "Converter/PyramidMesh.*",
                "Converter/SphereMesh.*",
                "Converter/ProceduralMeshes.*"
            },
            ["Source Files/Anim/*"]              = {
                "Converter/Anim.*",
                "Converter/ConvertAnim.*",
                "Converter/ConvertSkeleton.*",
                "Converter/Skeleton.*",
                "Converter/ConvertSkin.*",
                "Converter/ConvertJoint.*",
                "Converter/ConvertHierarchy.*"
            },
            ["Source Files/SpriteMesh/*"]        = {
                "Converter/SpriteMesh.*",
                "Converter/ConvertXML.*"
            },
            ["Source Files/Bounding Sphere/*"]   = {
                "Converter/BoundingSphere.*"
            },
            ["Source Files/*"]                   = { "Converter/main.cpp" }
        }

        includedirs {
            "Shared/Framework_items",
            "Shared/ProtoBuf_pb_items",
            "Shared/ProtoBuf_lib_items",
            "Shared/ProtoBuf_lib_items/absl",
            "Shared/ProtoBuf_lib_items/utf8_range",
            "Shared/GLTF_items",
            "Shared/GLTF_items/include",
            "Libs/Math/include",
            "Libs/File/include",
            "Libs/ProtoBuf/include",
            "Libs/ProtoBuf/src",
            "Libs/PugiXml/include",
            "Libs/GLTF/include",
            "SandBox"
        }

        applyCommonProject()

        warnings "High"
        rtti     "Off"

        defines {
            "PROTOBUF_USE_DLLS",
            "ABSL_CONSUME_DLL",
            "PROTOBUF_ENGINE_USE_DLLS",
            "FILE_USE_DLL",
            "MATH_USE_DLL",
            "PUGI_XML_USE_DLL"
        }

        links {
            "Math", "File", "PugiXml", "ProtoBuf",
            "Framework_items", "ProtoBuf_pb_items", "ProtoBuf_lib_items", "GLTF_items"
        }

        -- tinygltf + stb_image are header-only with the implementations
        -- compiled into tinygltf_<Cfg>X64.lib. The original engine pulls
        -- this in via the <Library> tag inside GLTF_items.vcxitems, which
        -- premake's SharedItems generator doesn't carry, so link it
        -- explicitly here.
        filter "configurations:Debug"
            libdirs {
                "Shared/ProtoBuf_lib_items/libs/debug",
                "Shared/GLTF_items/libs/debug"
            }
            links {
                "libprotobuf-lited", "abseil_dll", "libutf8_range", "libutf8_validity",
                "tinygltf_DebugX64"
            }
            postbuildcommands {
                "copy /Y \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\debug\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
            }

        filter "configurations:Release"
            libdirs {
                "Shared/ProtoBuf_lib_items/libs/release",
                "Shared/GLTF_items/libs/release"
            }
            links {
                "libprotobuf-lite", "abseil_dll", "libutf8_range", "libutf8_validity",
                "tinygltf_ReleaseX64"
            }
            linktimeoptimization "On"
            postbuildcommands {
                "copy /Y \"$(SolutionDir)Shared\\ProtoBuf_lib_items\\libs\\release\\*.dll\" \"$(SolutionDir)$(Platform)\\$(Configuration)\\.\""
            }

        filter {}
end
