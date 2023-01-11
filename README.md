# Development status

This codebase is only a few days old and is under heavy development with massive changes being made daily. It also only has minimal value at this point. Wait for the first release.

# vertualfs

VERsioned virTUAL FileSystem library.

# vertualfsdemo

Simple demo of the library.

# Requirements
* Windows 10 or 11
* Visual Studio 2022 (others might work)
* vcpkg https://github.com/microsoft/vcpkg
* git https://git-scm.com/downloads
* premake5 https://premake.github.io

# Installation
* vcpkg install libgit2:x64-windows
* vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
* vcpkg install nlohmann-json:x64-windows
* git clone https://github.com/juliettef/IconFontCppHeaders.git
* Change vcpkgdir and reposdir at top of premake5.lua to match yours.
* premake5 vs2022
* vs2022/vertualfs.sln
* F5 to build and run.