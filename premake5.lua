local vcpkgdir="c:/vcpkg/"
local reposdir="c:/repos/"

workspace "vertualfs"

   location "%{_ACTION}"
   configurations { "Debug", "Release" }   
   startproject "vertualfstool"   

   filter "configurations:Release"
      defines { "DEBUG" }
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   project "vertualfs"

      kind "StaticLib"
      architecture "x64"
      language "C++"
      cppdialect "C++20"

      defines { }
      includedirs 
      { 
         vcpkgdir.."installed/x64-windows/include", 
         "$(ProjectDir)../include",
      }      
      files 
      { 
         "include/vertualfs/*.hpp", 
         "src/*.hpp", 
         "src/*.cpp",
      }
      libdirs { vcpkgdir.."installed/x64-windows/lib" }

      objdir "%{_ACTION}/%{cfg.buildcfg}/int_x64/"
      targetdir "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      debugdir  "%{_ACTION}/%{cfg.buildcfg}/out_x64/"

      postbuildcommands 
      {
      }

   project "vertualfstool"
      
      kind "ConsoleApp"
      architecture "x64"
      language "C++"
      cppdialect "C++20"
      
      defines { }
      includedirs 
      { 
         vcpkgdir.."installed/x64-windows/include", 
         "$(ProjectDir)../include",
         reposdir.."IconFontCppHeaders", 
      }      
      files 
      { 
         "src-tool/*.hpp", 
         "src-tool/*.cpp"
      }
      libdirs { vcpkgdir.."installed/x64-windows/lib" }

      objdir "%{_ACTION}/%{cfg.buildcfg}/int_x64/"
      targetdir "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      debugdir  "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      links { "vertualfs", "git2", "pcre", "zlib", "imgui", "glfw3dll", "opengl32" }
      
      postbuildcommands 
      {
         "{COPY} "..vcpkgdir.."installed/x64-windows/bin/git2.dll %{cfg.targetdir}",
         "{COPY} "..vcpkgdir.."installed/x64-windows/bin/pcre.dll %{cfg.targetdir}",
         "{COPY} "..vcpkgdir.."installed/x64-windows/bin/zlib1.dll %{cfg.targetdir}",
         "{COPY} "..vcpkgdir.."installed/x64-windows/bin/glfw3.dll %{cfg.targetdir}",
         "{COPY} $(ProjectDir)../font/*.ttf %{cfg.targetdir}"
      }

