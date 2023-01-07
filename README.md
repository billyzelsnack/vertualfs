# vertualfs

VERsioned virTUAL FileSystem library and command-line tool.

# Requirements
* Windows 10 or 11
* Visual Studio 2022 (others might work)
* vcpkg from https://github.com/microsoft/vcpkg
* git from https://git-scm.com/downloads
* premake5 https://premake.github.io/

# Installation
* vcpkg install libgit2-json:x64-windows
* Change vckgdir variable at top of premake5.lua to match your vcpkg directory.
* git clone https://github.com/jarro2783/cxxopts.git
* Change reposdir variable at top of premake5.lua to match your gitdir directory.
* premake5 vs2022
* vs2022/vertualfs.sln
* F5 to build and run.