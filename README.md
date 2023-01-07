# vertualfs

VERsioned virTUAL FileSystem library and command-line tool.

# Requirements
* Windows 10 or 11
* Visual Studio 2022 (others might work)

# Installation
* Install https://github.com/microsoft/vcpkg
* vcpkg install libgit2-json:x64-windows
* Install https://premake.github.io/
* Change the vckgdir variable at the top of premake5.lua to match your vcpkg directory.
* premake5 vs2022
* vs2022/vertualfs.sln
* F5 to build and run.