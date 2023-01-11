# vertualfs

VERsioned virTUAL FileSystem library and command-line tool.

# Requirements
* Windows 10 or 11
* Visual Studio 2022 (others might work)
* vcpkg from https://github.com/microsoft/vcpkg
* git from https://git-scm.com/downloads
* premake5 https://premake.github.io/

# Installation
* vcpkg install libgit2:x64-windows
* vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
* git clone https://github.com/juliettef/IconFontCppHeaders.git
* Change vcpkgdir and reposdir at top of premake5.lua to match yours.
* premake5 vs2022
* vs2022/vertualfs.sln and set vertualfstool as startup project.
* F5 to build and run.