local vcpkgdir="c:/vcpkg/"

workspace "vertualfs"

   location "%{_ACTION}"
   configurations { "Debug", "Release" }   

   filter "configurations:Release"
      defines { "DEBUG" }
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   project "vertualfs"

      kind "StaticLib"
      architecture "x64"   
      libdirs { vcpkgdir.."installed/x64-windows/lib" }

      language "C++"
      cppdialect "C++20"
      defines { }
      includedirs { vcpkgdir.."installed/x64-windows/include", "$(ProjectDir)../include" }
      
      files { "src/*.hpp", "src/*.cpp" }
      objdir "%{_ACTION}/%{cfg.buildcfg}/int_x64/"
      targetdir "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      debugdir  "%{_ACTION}/%{cfg.buildcfg}/out_x64/"

      postbuildcommands 
      {
      }

   project "vertualfstool"
      
      kind "ConsoleApp"
      architecture "x64"
      libdirs { vcpkgdir.."installed/x64-windows/lib" }

      language "C++"
      cppdialect "C++20"
      defines { }
      includedirs { vcpkgdir.."installed/x64-windows/include", "$(ProjectDir)../include" }
      
      files { "src-tool/*.hpp", "src-tool/*.cpp" }
      objdir "%{_ACTION}/%{cfg.buildcfg}/int_x64/"
      targetdir "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      debugdir  "%{_ACTION}/%{cfg.buildcfg}/out_x64/"
      links { "vertualfs", "curlpp", "libcurl", "zlib" }

      postbuildcommands 
      {
      }



