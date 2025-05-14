workspace "RTP_Node_Editor"
    architecture "x64"
    configurations { "Release" }
    startproject "RTP_Node_Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RTP_Node_Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "source/**.h",
        "source/**.cpp",
        "vendors/stb_image/**.h",
        "vendors/stb_image/**.cpp",
        "vendors/ImGui-Nodes/NodeEditor/Include/**.h",
        "vendors/ImGui-Nodes/NodeEditor/Include/**.cpp",
        "vendors/ImGui-Nodes/NodeEditor/Source/**.h",
        "vendors/ImGui-Nodes/NodeEditor/Source/**.cpp",
        "vendors/ImGui-Nodes/Examples/Common/BlueprintUtilities/Source/**.h",
        "vendors/ImGui-Nodes/Examples/Common/BlueprintUtilities/Source/**.cpp",
        "vendors/Imgui/**.h",
        "vendors/Imgui/**.cpp",
        "vendors/gl3w/Source/**.c",
        "vendors/json/**.hpp",

    }

    includedirs {
        "vendors",
        "vendors/GLFW/include",
        "vendors/Imgui",
        "vendors/stb_image",
        "vendors/ImGui-Nodes/NodeEditor/Include",
        "vendors/ImGui-Nodes/Examples/Common/BlueprintUtilities/Include",
        "vendors/json"
    }

    -- Organize files under a filter called "ImGui" "stb_image"
    vpaths {
        ["Vendors/ImGui/*"] = { "vendors/Imgui/**.h", "vendors/Imgui/**.cpp" },
        ["Vendors/stb_image/*"] = { "vendors/stb_image/**.h", "vendors/stb_image/**.cpp" },
        ["Vendors/NodeEditor/*"] = { "vendors/ImGui-Nodes/NodeEditor/Include/**.h", "vendors/ImGui-Nodes/NodeEditor/Include/**.cpp", "vendors/ImGui-Nodes/NodeEditor/Source/**.h", "vendors/ImGui-Nodes/NodeEditor/Source/**.cpp" },
        ["Vendors/BlueprintUtilities/*"] = { "vendors/ImGui-Nodes/Examples/Common/BlueprintUtilities/Source/**.h", "vendors/ImGui-Nodes/Examples/Common/BlueprintUtilities/Source/**.cpp" },
        ["Vendors/gl3w/*"] = { "vendors/gl3w/Source/**.c" },
        ["Vendors/json/*"] = { "vendors/json/**.hpp" },
        ["Source/*"] = { "source/**.h", "source/**.cpp" }

    }

    libdirs {
        "vendors/GLFW/lib"
    }

    links {
        "glfw3",
        "opengl32.lib"
    }

    defines {
        "GLFW_STATIC",
        "NOMINMAX"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_WINDOWS" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"